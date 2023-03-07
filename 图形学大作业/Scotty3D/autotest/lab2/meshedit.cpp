
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>

#include "halfedge.h"

/******************************************************************
*********************** Local Operations **************************
******************************************************************/

/* Note on local operation return types:

    The local operations all return a std::optional<T> type. This is used so that your
    implementation can signify that it does not want to perform the operation for
    whatever reason (e.g. you don't want to allow the user to erase the last vertex).

    An optional can have two values: std::nullopt, or a value of the type it is
    parameterized on. In this way, it's similar to a pointer, but has two advantages:
    the value it holds need not be allocated elsewhere, and it provides an API that
    forces the user to check if it is null before using the value.

    In your implementation, if you have successfully performed the operation, you can
    simply return the required reference:

            ... collapse the edge ...
            return collapsed_vertex_ref;

    And if you wish to deny the operation, you can return the null optional:

            return std::nullopt;

    Note that the stubs below all reject their duties by returning the null optional.
*/

/* 
    This method splits the given edge in half, but does not split the
    adjacent faces. Returns an iterator to the new vertex which splits
    the original edge.

    Example function for how to go about implementing local operations
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::bisect_edge(EdgeRef e) {

    // Phase 1: collect all elements
    HalfedgeRef h = (e->halfedge()->is_boundary()) ? e->halfedge()->twin() : e->halfedge();
    HalfedgeRef ht = h->twin();
    HalfedgeRef preh = h;
    HalfedgeRef nexht = ht->next();
    do {
        preh = preh->next();
    } while (preh->next() != h);
    Vec3 vpos = (h->vertex()->pos + ht->vertex()->pos)/2;

    // Phase 2: Allocate new elements
    VertexRef c = new_vertex();
    c->pos = vpos;
    HalfedgeRef hn = new_halfedge();
    HalfedgeRef hnt = new_halfedge();
    EdgeRef e0 = new_edge();

    // The following elements aren't necessary for the bisect_edge, but they are here to demonstrate phase 4
    FaceRef f_not_used = new_face();
    HalfedgeRef h_not_used = new_halfedge();

    // Phase 3: Reassign elements
    e0->halfedge() = hn;
    hn->twin() = hnt;
    hn->edge() = e0;
    hn->vertex() = h->vertex();
    hn->face() = h->face();
    preh->next() = hn;
    hn->next() = h;
    h->vertex() = c;
    ht->next() = hnt;
    c->halfedge() = h;
    hn->vertex()->halfedge() = hn;
    // is_new parameter is used for global operations
    c->is_new = true;

    // example of set_neighbors:
    // condenses hnt->next() = nexht; hnt->twin() = hn; hnt->vertex() = c; hnt->edge() = e0; hnt->face() = ht->face(); into one line
    hnt->set_neighbors(nexht, hn, c, e0, ht->face());

    // Phase 4: Delete unused elements
    erase(f_not_used);
    erase(h_not_used);

    // Phase 5: Return the correct iterator
    return c;
}

/*
    This method should replace the given vertex and all its neighboring
    edges and faces with a single face, returning the new face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_vertex(Halfedge_Mesh::VertexRef v) {

    (void)v;
    return std::nullopt;
}

/*
    This method should erase the given edge and return an iterator to the
    merged face.
 */
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::erase_edge(Halfedge_Mesh::EdgeRef e) {

    (void)e;
    return std::nullopt;
}

/*
    This method should collapse the given edge and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_edge(Halfedge_Mesh::EdgeRef e) {

    (void)e;
    return std::nullopt;
}

/*
    This method should collapse the given face and return an iterator to
    the new vertex created by the collapse.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::collapse_face(Halfedge_Mesh::FaceRef f) {

    (void)f;
    return std::nullopt;
}

/*
    This method should flip the given edge counter-clockwise and return an iterator to the
    flipped edge.
*/
std::optional<Halfedge_Mesh::EdgeRef> Halfedge_Mesh::flip_edge(Halfedge_Mesh::EdgeRef e) {
    // halfedge to be managed
    Halfedge_Mesh::HalfedgeRef a = e->halfedge();
    Halfedge_Mesh::HalfedgeRef b = e->halfedge()->twin();
    // set face
    Halfedge_Mesh::FaceRef face_a = a->face();
    face_a->_halfedge = a;
    Halfedge_Mesh::FaceRef face_b = b->face();
    face_b->_halfedge = b;
    // get halfedge and vertex
    Halfedge_Mesh::HalfedgeRef a_next = a->next()->next();
    Halfedge_Mesh::HalfedgeRef a_next_next = b->next();
    Halfedge_Mesh::HalfedgeRef b_next = b->next()->next();
    Halfedge_Mesh::HalfedgeRef b_next_next = a->next();
    Halfedge_Mesh::VertexRef a_vertex = b_next->vertex();
    Halfedge_Mesh::VertexRef b_vertex = a_next->vertex();
    Halfedge_Mesh::VertexRef a_from = a->vertex();
    Halfedge_Mesh::VertexRef b_from = b->vertex();
    // renew all the halfedge
    a->set_neighbors(a_next, b, a_vertex, e, face_a);
    a_next->_next = a_next_next;
    a_next->_face = face_a;
    a_next_next->_next = a;
    a_next_next->_face = face_a;
    b->set_neighbors(b_next, a, b_vertex, e, face_b);
    b_next->_next = b_next_next;
    b_next->_face = face_b;
    b_next_next->_next = b;
    b_next_next->_face = face_b;
    // set vertex
    if(!a_from->is_new) a_from->_halfedge = a_next_next;
    if(!b_from->is_new) b_from->_halfedge = b_next_next;

    return e;
}

/*
    This method should split the given edge and return an iterator to the
    newly inserted vertex. The halfedge of this vertex should point along
    the edge that was split, rather than the new edges.
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::split_edge(Halfedge_Mesh::EdgeRef e) {
    // new elements
    Halfedge_Mesh::VertexRef new_v = new_vertex();
    Halfedge_Mesh::EdgeRef new_e = new_edge();
    Halfedge_Mesh::EdgeRef new_e1 = new_edge();
    Halfedge_Mesh::EdgeRef new_e2 = new_edge();
    Halfedge_Mesh::FaceRef new_f1 = new_face();
    Halfedge_Mesh::FaceRef new_f2 = new_face();
    Halfedge_Mesh::HalfedgeRef new_a = new_halfedge();
    Halfedge_Mesh::HalfedgeRef new_b = new_halfedge();
    Halfedge_Mesh::HalfedgeRef new_e1a = new_halfedge();
    Halfedge_Mesh::HalfedgeRef new_e1b = new_halfedge();
    Halfedge_Mesh::HalfedgeRef new_e2a = new_halfedge();
    Halfedge_Mesh::HalfedgeRef new_e2b = new_halfedge();
    // orginal vertex and edges
    Vec3 new_pos = e->center();
    Halfedge_Mesh::HalfedgeRef a = e->halfedge();
    Halfedge_Mesh::HalfedgeRef b = a->twin();
    Halfedge_Mesh::HalfedgeRef a_next = a->next();
    Halfedge_Mesh::HalfedgeRef a_next_next = a_next->next();
    Halfedge_Mesh::HalfedgeRef b_next = b->next();
    Halfedge_Mesh::HalfedgeRef b_next_next = b_next->next();
    Halfedge_Mesh::VertexRef target1 = a_next_next->vertex();
    Halfedge_Mesh::VertexRef target2 = b_next_next->vertex();
    Halfedge_Mesh::VertexRef new_a_from = a->vertex();
    Halfedge_Mesh::FaceRef face1 = a->face();
    Halfedge_Mesh::FaceRef face2 = b->face();

    // set halfedge around face1
    a->_vertex = new_v;
    a_next->_next = new_e1a;
    new_e1a->set_neighbors(a, new_e1b, target1, new_e1, face1);
    // set halfedge around face2
    b->_next = new_e2a;
    new_e2a->set_neighbors(b_next_next, new_e2b, new_v, new_e2, face2);
    // set halfedge around new_f1
    new_a->set_neighbors(new_e1b, new_b, new_a_from, new_e, new_f1);
    new_e1b->set_neighbors(a_next_next, new_e1a, new_v, new_e1, new_f1);
    a_next_next->_face = new_f1;
    a_next_next->_next = new_a;
    // set halfedge around new_f2
    new_b->set_neighbors(b_next, new_a, new_v, new_e, new_f2);
    b_next->_face = new_f2;
    b_next->_next = new_e2b;
    new_e2b->set_neighbors(new_b, new_e2a, target2, new_e2, new_f2);

    // set faces
    face1->_halfedge = a;
    face2->_halfedge = b;
    new_f1->_halfedge = new_a;
    new_f2->_halfedge = new_b;
    // set edges
    new_e->_halfedge = new_a;
    new_e1->_halfedge = new_e1a;
    new_e2->_halfedge = new_e2a;
    new_e1->is_new = true;
    new_e2->is_new = true;
    // set vertex
    new_v->_halfedge = a;
    EdgeRef edge = new_v->halfedge()->edge();
    // info("new_v edge:%d", edge->id());
    // info("new_v pos : (%f,%f,%f)", edge->new_pos.x, edge->new_pos.y, edge->new_pos.z);

    new_v->is_new = true;
    // a->_edge = e;
    new_v->pos = new_pos;
    new_a_from->_halfedge = new_a;

    return new_v;
}


/*
    This method should insets a vertex into the given face, returning a pointer to the new center vertex
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::inset_vertex(FaceRef f) {
    (void)f;
    return std::nullopt;
}

/*
    This method should inset a face into the given face, returning a pointer to the new face.
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::inset_face(Halfedge_Mesh::FaceRef f) {

    // hint: use bevel_face positions as a helper function here
    (void)f;
    return std::nullopt;
}

/*
    This method should bevel a vertex and inserts a vertex into the new vertex, returning a pointer to that vertex
*/
std::optional<Halfedge_Mesh::VertexRef> Halfedge_Mesh::extrude_vertex(VertexRef v) {
    (void)v;
    return std::nullopt;
}

/* Note on the beveling process:

    Each of the bevel_vertex, bevel_edge, and bevel_face functions do not represent
    a full bevel operation. Instead, they should update the _connectivity_ of
    the mesh, _not_ the positions of newly created vertices. In fact, you should set
    the positions of new vertices to be exactly the same as wherever they "started from."

    When you click on a mesh element while in bevel mode, one of those three functions
    is called. But, because you may then adjust the distance/offset of the newly
    beveled face, we need another method of updating the positions of the new vertices.

    This is where bevel_vertex_positions, bevel_edge_positions, and
    bevel_face_positions come in: these functions are called repeatedly as you
    move your mouse, the position of which determines the normal and tangent offset
    parameters. These functions are also passed an array of the original vertex
    positions: for bevel_vertex, it has one element, the original vertex position,
    for bevel_edge, two for the two vertices, and for bevel_face, it has the original
    position of each vertex in order starting from face->halfedge. You should use these 
    positions, as well as the normal and tangent offset fields to assign positions to 
    the new vertices.

    Finally, note that the normal and tangent offsets are not relative values - you
    should compute a particular new position from them, not a delta to apply.
*/

/*
    This method should replace the vertex v with a face, corresponding to
    a bevel operation. It should return the new face.  NOTE: This method is
    only responsible for updating the *connectivity* of the mesh---it does not
    need to update the vertex positions. These positions will be updated in
    Halfedge_Mesh::bevel_vertex_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_vertex(Halfedge_Mesh::VertexRef v) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)v;
    return std::nullopt;
}

/*
    This method should replace the edge e with a face, corresponding to a
    bevel operation. It should return the new face. NOTE: This method is
    responsible for updating the *connectivity* of the mesh only---it does not
    need to update the vertex positions. These positions will be updated in
    Halfedge_Mesh::bevel_edge_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_edge(Halfedge_Mesh::EdgeRef e) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)e;
    return std::nullopt;
}

/*
    This method should replace the face f with an additional, inset face
    (and ring of faces around it), corresponding to a bevel operation. It
    should return the new face.  NOTE: This method is responsible for updating
    the *connectivity* of the mesh only---it does not need to update the vertex
    positions. These positions will be updated in
    Halfedge_Mesh::bevel_face_positions (which you also have to
    implement!)
*/
std::optional<Halfedge_Mesh::FaceRef> Halfedge_Mesh::bevel_face(Halfedge_Mesh::FaceRef f) {

    // Reminder: You should set the positions of new vertices (v->pos) to be exactly
    // the same as wherever they "started from."

    (void)f;
    return std::nullopt;
}

/*
    Compute new vertex positions for the vertices of the beveled vertex.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the original vertex position and its associated outgoing edge
    to compute a new vertex position along the outgoing edge.
*/
void Halfedge_Mesh::bevel_vertex_positions(const std::vector<Vec3>& start_positions,
                                           Halfedge_Mesh::FaceRef face, float tangent_offset) {

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled edge.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 1, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the orig array) to compute an offset vertex position.

    Note that there is a 1-to-1 correspondence between halfedges in
    newHalfedges and vertex positions in start_positions. So, you can write 
    loops of the form:

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vector3D pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_edge_positions(const std::vector<Vec3>& start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset) {

    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
}

/*
    Compute new vertex positions for the vertices of the beveled face.

    These vertices can be accessed via new_halfedges[i]->vertex()->pos for
    i = 0, ..., new_halfedges.size()-1.

    The basic strategy here is to loop over the list of outgoing halfedges,
    and use the preceding and next vertex position from the original mesh
    (in the start_positions array) to compute an offset vertex
    position.

    Note that there is a 1-to-1 correspondence between halfedges in
    new_halfedges and vertex positions in start_positions. So, you can write 
    loops of the form:

    for(size_t i = 0; i < new_halfedges.size(); i++)
    {
            Vec3 pi = start_positions[i]; // get the original vertex
            position corresponding to vertex i
    }
*/
void Halfedge_Mesh::bevel_face_positions(const std::vector<Vec3>& start_positions,
                                         Halfedge_Mesh::FaceRef face, float tangent_offset,
                                         float normal_offset) {

    if(flip_orientation) normal_offset = -normal_offset;
    std::vector<HalfedgeRef> new_halfedges;
    auto h = face->halfedge();
    do {
        new_halfedges.push_back(h);
        h = h->next();
    } while(h != face->halfedge());

    (void)new_halfedges;
    (void)start_positions;
    (void)face;
    (void)tangent_offset;
    (void)normal_offset;
}

/*
    Updates the position of v using the given start_position
*/
void Halfedge_Mesh::extrude_vertex_position(const Vec3& start_positions, Halfedge_Mesh::FaceRef face) {
    (void)start_positions;
    (void)face;
}

/******************************************************************
*********************** Global Operations *************************
******************************************************************/

/*
    Splits all non-triangular faces into triangles.
*/
void Halfedge_Mesh::triangulate() {

    // For each face...
}

/* Note on the quad subdivision process:

        Unlike the local mesh operations (like bevel or edge flip), we will perform
        subdivision by splitting *all* faces into quads "simultaneously."  Rather
        than operating directly on the halfedge data structure (which as you've
        seen is quite difficult to maintain!) we are going to do something a bit nicer:
           1. Create a raw list of vertex positions and faces (rather than a full-
              blown halfedge mesh).
           2. Build a new halfedge mesh from these lists, replacing the old one.
        Sometimes rebuilding a data structure from scratch is simpler (and even
        more efficient) than incrementally modifying the existing one.  These steps are
        detailed below.

  Step I: Compute the vertex positions for the subdivided mesh.
        Here we're going to do something a little bit strange: since we will
        have one vertex in the subdivided mesh for each vertex, edge, and face in
        the original mesh, we can nicely store the new vertex *positions* as
        attributes on vertices, edges, and faces of the original mesh. These positions
        can then be conveniently copied into the new, subdivided mesh.
        This is what you will implement in linear_subdivide_positions() and
        catmullclark_subdivide_positions().

  Steps II-IV are provided (see Halfedge_Mesh::subdivide()), but are still detailed
  here:

  Step II: Assign a unique index (starting at 0) to each vertex, edge, and
        face in the original mesh. These indices will be the indices of the
        vertices in the new (subdivided) mesh. They do not have to be assigned
        in any particular order, so long as no index is shared by more than one
        mesh element, and the total number of indices is equal to V+E+F, i.e.,
        the total number of vertices plus edges plus faces in the original mesh.
        Basically we just need a one-to-one mapping between original mesh elements
        and subdivided mesh vertices.

  Step III: Build a list of quads in the new (subdivided) mesh, as tuples of
        the element indices defined above. In other words, each new quad should be
        of the form (i,j,k,l), where i,j,k and l are four of the indices stored on
        our original mesh elements.  Note that it is essential to get the orientation
        right here: (i,j,k,l) is not the same as (l,k,j,i).  Indices of new faces
        should circulate in the same direction as old faces (think about the right-hand
        rule).

  Step IV: Pass the list of vertices and quads to a routine that clears
        the internal data for this halfedge mesh, and builds new halfedge data from
        scratch, using the two lists.
*/

/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos.  The values of the positions are based on
    simple linear interpolation, e.g., the edge midpoints and face
    centroids.
*/
void Halfedge_Mesh::linear_subdivide_positions() {

    // For each vertex, assign Vertex::new_pos to
    // its original position, Vertex::pos.

    // For each edge, assign the midpoint of the two original
    // positions to Edge::new_pos.

    // For each face, assign the centroid (i.e., arithmetic mean)
    // of the original vertex positions to Face::new_pos. Note
    // that in general, NOT all faces will be triangles!
}

/*
    Compute new vertex positions for a mesh that splits each polygon
    into quads (by inserting a vertex at the face midpoint and each
    of the edge midpoints).  The new vertex positions will be stored
    in the members Vertex::new_pos, Edge::new_pos, and
    Face::new_pos. The values of the positions are based on
    the Catmull-Clark rules for subdivision.

    Note: this will only be called on meshes without boundary
*/
void Halfedge_Mesh::catmullclark_subdivide_positions() {

    // The implementation for this routine should be
    // a lot like Halfedge_Mesh:linear_subdivide_positions:(),
    // except that the calculation of the positions themsevles is
    // slightly more involved, using the Catmull-Clark subdivision
    // rules. (These rules are outlined in the Developer Manual.)

    // Faces

    // Edges

    // Vertices
}

/*
    This routine should increase the number of triangles in the mesh
    using Loop subdivision. Note: this is will only be called on triangle meshes.
*/
void Halfedge_Mesh::loop_subdivide() {

    // Each vertex and edge of the original mesh can be associated with a
    // vertex in the new (subdivided) mesh.
    // Therefore, our strategy for computing the subdivided vertex locations is to
    // *first* compute the new positions
    // using the connectivity of the original (coarse) mesh. Navigating this mesh
    // will be much easier than navigating
    // the new subdivided (fine) mesh, which has more elements to traverse.  We
    // will then assign vertex positions in
    // the new mesh based on the values we computed for the original mesh.

    // Compute new positions for all the vertices in the input mesh using
    // the Loop subdivision rule and store them in Vertex::new_pos.
    //    At this point, we also want to mark each vertex as being a vertex of the
    //    original mesh. Use Vertex::is_new for this.
    for(VertexRef v = vertices_begin(); v != vertices_end(); ++v) {
        Vec3 old = v->center();
        Vec3 sum;
        int count = 0;
        HalfedgeCRef h = v->halfedge();
        do {
            VertexCRef neighbour = h->twin()->vertex();
            sum += neighbour->center();
            count++;
            h = h->twin()->next();
        } while(h != v->halfedge());
        if(count == 3)
            v->new_pos = 7.0f / 16.0f * v->center() + 3.0f / 16.0f * sum;
        else
            v->new_pos = 5.0f / 8.0f * v->center() + 3.0f / 8.0f / count * sum;
        // info("vertex new_pos:(%f,%f,%f)", v->new_pos.x, v->new_pos.y, v->new_pos.z);
        v->is_new = false;
    }

    // Next, compute the subdivided vertex positions associated with edges, and
    // store them in Edge::new_pos.
    for(EdgeRef edge = edges_begin(); edge != edges_end(); ++edge) {
        VertexCRef v1, v2, v3, v4;
        v1 = edge->halfedge()->vertex();
        v2 = edge->halfedge()->twin()->vertex();
        v3 = edge->halfedge()->next()->next()->vertex();
        v4 = edge->halfedge()->twin()->next()->next()->vertex();
        // info("edge:%d", edge->id());
        // info("position: (%d,%d,%d,%d)", v1->id(), v2->id(), v3->id(), v4->id());
        // info("v1:(%f,%f,%f)", v1->center().x, v1->center().y, v1->center().z);
        // info("v2:(%f,%f,%f)", v2->center().x, v2->center().y, v2->center().z);
        // info("v3:(%f,%f,%f)", v3->center().x, v3->center().y, v3->center().z);
        // info("v4:(%f,%f,%f)", v4->center().x, v4->center().y, v4->center().z);
        Vec3 new_p = 3.0f / 8.0f * (v1->center() + v2->center()) +
                     1.0f / 8.0f * (v3->center() + v4->center());
        // info("new_p:(%f,%f,%f)", new_p.x, new_p.y, new_p.z);
        edge->new_pos = new_p;
        edge->is_new = false;
        // info("edge new_pos : (%f,%f,%f)", edge->new_pos.x, edge->new_pos.y, edge->new_pos.z);
    }

    // Next, we're going to split every edge in the mesh, in any order.
    // We're also going to distinguish subdivided edges that came from splitting
    // an edge in the original mesh from new edges by setting the boolean Edge::is_new.
    // Note that in this loop, we only want to iterate over edges of the original mesh.
    // Otherwise, we'll end up splitting edges that we just split (and the
    // loop will never end!)
    // info("----------split edge------------");
    int num = static_cast<int>(edges.size());
    for(EdgeRef edge = edges_begin(); num; num--) {
        // info("edge:%d", edge->id());
        split_edge(edge);
        // info("edge new_pos : (%f,%f,%f)", edge->new_pos.x, edge->new_pos.y, edge->new_pos.z);
        edge++;
    }

    // Now flip any new edge that connects an old and new vertex.
    for(EdgeRef edge = edges_begin(); edge != edges_end(); ++edge) {
        if(edge->is_new == false) continue;
        VertexCRef v1 = edge->halfedge()->vertex();
        VertexCRef v2 = edge->halfedge()->twin()->vertex();
        if(v2->is_new && v1->is_new) continue;
        flip_edge(edge);
    }
    // Finally, copy new vertex positions into the Vertex::pos.
    // info("---------------------set new vertex-------------------");
    for(VertexRef v = vertices_begin(); v != vertices_end(); ++v) {
        if(v->is_new == false) {
            // info("old vertex:(%f,%f,%f)", v->new_pos.x, v->new_pos.y, v->new_pos.z);
            v->pos = v->new_pos;
        } else {
            EdgeRef edge = v->halfedge()->edge();
            // info("edge:%d", edge->id());
            // info("new_vertex : (%f,%f,%f)", edge->new_pos.x, edge->new_pos.y, edge->new_pos.z);
            v->pos = edge->new_pos;
        }
    }
}

/*
    Isotropic remeshing. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if this is not a triangle mesh)
*/
bool Halfedge_Mesh::isotropic_remesh() {

    // Compute the mean edge length.
    // Repeat the four main steps for 5 or 6 iterations
    // -> Split edges much longer than the target length (being careful about
    //    how the loop is written!)
    // -> Collapse edges much shorter than the target length.  Here we need to
    //    be EXTRA careful about advancing the loop, because many edges may have
    //    been destroyed by a collapse (which ones?)
    // -> Now flip each edge if it improves vertex degree
    // -> Finally, apply some tangential smoothing to the vertex positions

    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate is called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return false;
}

/* Helper type for quadric simplification */
struct Edge_Record {
    Edge_Record() {
    }
    Edge_Record(std::unordered_map<Halfedge_Mesh::VertexRef, Mat4>& vertex_quadrics,
                Halfedge_Mesh::EdgeRef e)
        : edge(e) {

        // Compute the combined quadric from the edge endpoints.
        // -> Build the 3x3 linear system whose solution minimizes the quadric error
        //    associated with these two endpoints.
        // -> Use this system to solve for the optimal position, and store it in
        //    Edge_Record::optimal.
        // -> Also store the cost associated with collapsing this edge in
        //    Edge_Record::cost.
    }
    Halfedge_Mesh::EdgeRef edge;
    Vec3 optimal;
    float cost;
};

/* Comparison operator for Edge_Records so std::set will properly order them */
bool operator<(const Edge_Record& r1, const Edge_Record& r2) {
    if(r1.cost != r2.cost) {
        return r1.cost < r2.cost;
    }
    Halfedge_Mesh::EdgeRef e1 = r1.edge;
    Halfedge_Mesh::EdgeRef e2 = r2.edge;
    return &*e1 < &*e2;
}

/** Helper type for quadric simplification
 *
 * A PQueue is a minimum-priority queue that
 * allows elements to be both inserted and removed from the
 * queue.  Together, one can easily change the priority of
 * an item by removing it, and re-inserting the same item
 * but with a different priority.  A priority queue, for
 * those who don't remember or haven't seen it before, is a
 * data structure that always keeps track of the item with
 * the smallest priority or "score," even as new elements
 * are inserted and removed.  Priority queues are often an
 * essential component of greedy algorithms, where one wants
 * to iteratively operate on the current "best" element.
 *
 * PQueue is templated on the type T of the object
 * being queued.  For this reason, T must define a comparison
 * operator of the form
 *
 *    bool operator<( const T& t1, const T& t2 )
 *
 * which returns true if and only if t1 is considered to have a
 * lower priority than t2.
 *
 * Basic use of a PQueue might look
 * something like this:
 *
 *    // initialize an empty queue
 *    PQueue<myItemType> queue;
 *
 *    // add some items (which we assume have been created
 *    // elsewhere, each of which has its priority stored as
 *    // some kind of internal member variable)
 *    queue.insert( item1 );
 *    queue.insert( item2 );
 *    queue.insert( item3 );
 *
 *    // get the highest priority item currently in the queue
 *    myItemType highestPriorityItem = queue.top();
 *
 *    // remove the highest priority item, automatically
 *    // promoting the next-highest priority item to the top
 *    queue.pop();
 *
 *    myItemType nextHighestPriorityItem = queue.top();
 *
 *    // Etc.
 *
 *    // We can also remove an item, making sure it is no
 *    // longer in the queue (note that this item may already
 *    // have been removed, if it was the 1st or 2nd-highest
 *    // priority item!)
 *    queue.remove( item2 );
 *
 */
template<class T> struct PQueue {
    void insert(const T& item) {
        queue.insert(item);
    }
    void remove(const T& item) {
        if(queue.find(item) != queue.end()) {
            queue.erase(item);
        }
    }
    const T& top(void) const {
        return *(queue.begin());
    }
    void pop(void) {
        queue.erase(queue.begin());
    }
    size_t size() {
        return queue.size();
    }

    std::set<T> queue;
};

/*
    Mesh simplification. Note that this function returns success in a similar
    manner to the local operations, except with only a boolean value.
    (e.g. you may want to return false if you can't simplify the mesh any
    further without destroying it.)
*/
bool Halfedge_Mesh::simplify() {

    std::unordered_map<VertexRef, Mat4> vertex_quadrics;
    std::unordered_map<FaceRef, Mat4> face_quadrics;
    std::unordered_map<EdgeRef, Edge_Record> edge_records;
    PQueue<Edge_Record> edge_queue;

    // Compute initial quadrics for each face by simply writing the plane equation
    // for the face in homogeneous coordinates. These quadrics should be stored
    // in face_quadrics
    // -> Compute an initial quadric for each vertex as the sum of the quadrics
    //    associated with the incident faces, storing it in vertex_quadrics
    // -> Build a priority queue of edges according to their quadric error cost,
    //    i.e., by building an Edge_Record for each edge and sticking it in the
    //    queue. You may want to use the above PQueue<Edge_Record> for this.
    // -> Until we reach the target edge budget, collapse the best edge. Remember
    //    to remove from the queue any edge that touches the collapsing edge
    //    BEFORE it gets collapsed, and add back into the queue any edge touching
    //    the collapsed vertex AFTER it's been collapsed. Also remember to assign
    //    a quadric to the collapsed vertex, and to pop the collapsed edge off the
    //    top of the queue.

    // Note: if you erase elements in a local operation, they will not be actually deleted
    // until do_erase or validate are called. This is to facilitate checking
    // for dangling references to elements that will be erased.
    // The rest of the codebase will automatically call validate() after each op,
    // but here simply calling collapse_edge() will not erase the elements.
    // You should use collapse_edge_erase() instead for the desired behavior.

    return false;
}
