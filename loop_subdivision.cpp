#include <utility>
#include <unordered_map>
#include <cassert>
#include <numeric>

struct Mesh
{
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector> uvs;
	std::vector<Triangle> triangles;
	
	void loop_subdivision();
};

typedef std::pair<int, int> Edge;// edge is pair from indexes of vertices 

auto make_edge(int lhs, int rhs)
{
	if (lhs > rhs)
		std::swap(lhs, rhs);

	return std::make_pair(lhs, rhs);
}

struct EdgeInfo
{
	EdgeInfo() {}

	EdgeInfo(const int index_triangle, const int index_of_opposite_vertex)
		:t_first(index_triangle)
		, v_first(index_of_opposite_vertex)
		, t_second(-1)
		, v_second(-1)
	{}

	EdgeInfo(const int index_triangle, const int index_of_opposite_vertex, const EdgeInfo& info)
		: t_first(index_triangle)
		, v_first(index_of_opposite_vertex)
		, t_second(info.t_first)
		, v_second(info.v_first)
	{
		assert(info.t_second == -1);
		assert(info.v_second == -1);
	}

	union
	{
		//each edge in mesh participates in one or two triangles
		struct
		{
			int t_first;//index of triangle
			int t_second;
			int v_first;//index of edge opposite vertex
			int v_second;
		};
		struct
		{
			// new vertex for each edge
			int index_vertex;
			Vector normal;
		};
	};
};

struct Hash
{
	std::hash<int> h;
	
	int operator()(const Edge &e) const
	{
		return 13*h(e.first) + 7*h(e.second);
	}
};

typedef std::unordered_map<Edge, EdgeInfo, Hash> Map;

inline auto get_edges(const std::vector<Triangle>& triangles)
{
	Map edge_to_info;

	for (size_t j = 0; j < triangles.size(); ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			const auto edge = make_edge(triangles[j].v[i], triangles[j].v[(i + 1) % 3]);
			const auto it   = edge_to_info.find(edge);

			edge_to_info.insert_or_assign(
				edge,
				it == edge_to_info.end() ?
				EdgeInfo(j, (i + 2) % 3) :
				EdgeInfo(j, (i + 2) % 3, it->second));
		}
	}
	return edge_to_info;
}

inline auto compile_vertex(const Vector& v1, const Vector& v2)
{
	//boundary edge
	return (v1 + v2)*0.5;
}

inline auto compile_vertex(const Vector& v1, const Vector& v2, const Vector& v3, const Vector& v4)
{
	return (3.0 / 8.0)*(v1 + v2) + (1.0 / 8.0)*(v3 + v4);
}

inline void add_neighbour(std::vector<std::pair<Vector, int>>& neighbours, const int index, const Vector& neighbour)
{
	neighbours[index].first += neighbour;
	++neighbours[index].second;
}

auto compute_odd_vertices(Map& edges, Mesh& mesh)
{
	std::vector<std::pair<Vector, int>> neighbours(mesh.vertices.size(), std::make_pair(Vector{ 0,0,0 }, 0));
	
	for(auto& it : edges)
	{
		const auto& edge = it.first;
		auto& info = it.second;

		const auto& v1 = mesh.vertices[edge.first];
		const auto& v2 = mesh.vertices[edge.second];

		int normals[4];
		int n = 2;
		for(int i = 0; i < 2; ++i)
			normals[i] = mesh.triangles[info.t_first].n[(info.v_first + i + 1) % 3];

		add_neighbour(neighbours, edge.first, v2);
		add_neighbour(neighbours, edge.second, v1);

		if (info.t_second == -1)
		{
			mesh.vertices.push_back(compile_vertex(v1,v2));
		}
		else
		{
			const auto& v3 = mesh.vertices[mesh.triangles[info.t_first].v[info.v_first]];
			const auto& v4 = mesh.vertices[mesh.triangles[info.t_second].v[info.v_second]];

			for (int i = 0; i < 2; ++i)
				normals[i + 2] = mesh.triangles[info.t_second].n[(info.v_second + i + 1) % 3];

			n = 4;

			mesh.vertices.push_back(compile_vertex(v1, v2, v3, v4));
		}
		info.index_vertex = mesh.vertices.size() - 1;

		info.normal.makeZero();
		info.normal = std::accumulate(normals, normals + n, info.normal, [&mesh](const auto init, const auto index)
		{
			return init + mesh.normals[index];
		});
		info.normal /= double(n);
	}
	return neighbours;
}

inline void set_vertex_in_triangle(Triangle& t, const int index, const int vertex, const int normal, const int uvs)
{
	t.v[index] = vertex;
	t.n[index] = normal;
	t.t[index] = uvs;
}

inline auto new_triangles(Map& edge_to_vertex, Mesh& mesh)
{
	const double alpha = 0.32;

	decltype(mesh.triangles) new_triangles;
	EdgeInfo info[3];
	int normals[3];
	int uvs[3];

	for (const auto & triangle : mesh.triangles)
	{
		for (int i = 0; i < 3; ++i)
		{
			const auto index1 = (i + 1) % 3;
			const auto index2 = (i + 2) % 3;

			info[i] = edge_to_vertex.at(make_edge(triangle.v[index1], triangle.v[index2]));

			normals[i] = mesh.normals.size();
			mesh.normals.push_back(
				alpha * 0.5 * (mesh.normals[triangle.n[index1]] + mesh.normals[triangle.n[index2]]) 
				+ (1 - alpha) * info[i].normal);

			uvs[i] = mesh.uvs.size();
			mesh.uvs.push_back(0.5 * (mesh.uvs[triangle.t[index1]] + mesh.uvs[triangle.t[index2]]));
		}
		for (int i = 0; i < 3; ++i)
		{
			new_triangles.push_back(triangle);

			const auto pos1 = (i + 1) % 3;
			const auto pos2 = (i + 2) % 3;
			
			set_vertex_in_triangle(new_triangles.back(), pos1, info[pos2].index_vertex, normals[pos2], uvs[pos2]);
			set_vertex_in_triangle(new_triangles.back(), pos2, info[pos1].index_vertex, normals[pos1], uvs[pos1]);
		}

		new_triangles.push_back({});
		for (int i = 0; i < 3; ++i)
			set_vertex_in_triangle(new_triangles.back(), i, info[i].index_vertex, normals[i], uvs[i]);
	}

	return new_triangles;
}

inline void compure_even_vertex(std::vector<Vector> &vertices, const std::vector<std::pair<Vector, int>>& neighbours)
{
	std::transform(neighbours.cbegin(), neighbours.cend(), vertices.cbegin(), vertices.begin(),[](const auto &p, const auto&v){
		switch (p.second)
		{
		case 0: 
			return v;
		case 2: 
			return 0.75*v + (1.0 / 8.0)*p.first;
		default:
			const auto beta = p.second == 3 ? 3.0 / 16.0 : 3.0 / (8.0 * p.second);
			return (1.0 - p.second*beta)*v + beta * p.first;
		}
	});
}

void Mesh::loop_subdivision()
{
	auto edge_to_vertex = get_edges(triangles);
	{
		//neighbours.size == even_vertex.size
		const auto neighbours = compute_odd_vertices(edge_to_vertex, *this);
		compure_even_vertex(vertices, neighbours);
	}
	triangles = new_triangles(edge_to_vertex, *this);
}