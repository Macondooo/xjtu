#pragma once

#include <cstddef>
#include <cstdio>

#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <optional>

#include "../../src/lib/log.h"
#include "halfedge.h"

using std::size_t;
using std::pair, std::tuple, std::string;
using std::vector, std::set, std::unordered_map;
using std::optional;

using HalfedgeCRef = Halfedge_Mesh::HalfedgeCRef;
using VertexCRef = Halfedge_Mesh::VertexCRef;
using EdgeCRef = Halfedge_Mesh::EdgeCRef;
using Index = Halfedge_Mesh::Index;

using EdgePair = pair<size_t, size_t>;

tuple<vector<Vec3>, set<EdgePair>> extract(const Halfedge_Mesh& mesh);

optional<unordered_map<size_t, size_t>>
test_and_map_vertices(const vector<Vec3>& std, const vector<Vec3>& stu);

bool test_edges(
    const set<EdgePair>& std,
    const set<EdgePair>& stu,
    const unordered_map<size_t, size_t>& std_to_stu
);

tuple<vector<Vec3>, set<EdgePair>> load_std(const string &name);
