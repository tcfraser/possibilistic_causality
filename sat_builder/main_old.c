#include <stdio.h>
#define MAX_NUM_VERTICES 16
#define MAX_NUM_EDGES    16

typedef unsigned int uint;

typedef enum { false, true } bool;

typedef struct {
    bool visible;
    uint label;
} Vertex;

typedef struct {
    Vertex parent;
    Vertex child;
} Edge;

typedef struct {
    uint len;
    Vertex vertices[MAX_NUM_VERTICES];
} VertexList;

typedef struct {
    uint len;
    uint sizes[MAX_NUM_VERTICES];
} Cardinalities;

typedef struct {
    uint len;
    Edge edges[MAX_NUM_EDGES];
} EdgeList;

typedef struct {
    VertexList vl;
    EdgeList el;
} Graph;

typedef struct {
    Vertex principle;
    VertexList parents;
    Cardinalities cardinalities;
} SymbolicResponseTensor ;

/* function declaration */
bool vertexEqualQ( Vertex v1, Vertex v2 );
char vertexToChar( Vertex v );
// void printIntToCharConversionTable( );
void printEdge( Edge edge );
void printVertexList( VertexList vertexList );
VertexList getParents(Vertex var, Graph g);

// /* function declaration */
// void getParents( struct Edge * edge, int len ) {

// }

int main() {

    // printIntToCharConversionTable();

    printf("sizeof(char)        = %i\n", sizeof(char));
    printf("sizeof(int)         = %i\n", sizeof(int));
    printf("sizeof(Vertex)      = %i\n", sizeof(Vertex));
    printf("sizeof(Edge)        = %i\n", sizeof(Edge));
    printf("sizeof(VertexList)  = %i\n", sizeof(VertexList));
    printf("sizeof(EdgeList)    = %i\n", sizeof(EdgeList));
    printf("sizeof(Graph)       = %i\n", sizeof(Graph));

    Vertex v = {1, 1};

    Edge e = {{1, 0}, {1, 1}};
    printEdge(e);

    printf("vertexToChar((Vertex) {1, 0}) = %c\n", vertexToChar((Vertex) {1, 0}));
    printf("vertexToChar((Vertex) {1, 1}) = %c\n", vertexToChar((Vertex) {1, 1}));
    printf("vertexToChar((Vertex) {1, 2}) = %c\n", vertexToChar((Vertex) {1, 2}));
    printf("vertexToChar((Vertex) {0, 0}) = %c\n", vertexToChar((Vertex) {0, 0}));
    printf("vertexToChar((Vertex) {0, 1}) = %c\n", vertexToChar((Vertex) {0, 1}));
    printf("vertexToChar((Vertex) {0, 2}) = %c\n", vertexToChar((Vertex) {0, 2}));

    // struct VertexList vl1;
    // uint x;

    // printf("%u\n", vl1.len);
    // printf("%u\n", x);

    VertexList vl = {6, {{1, 0}, {1, 1}, {1, 2}, {0, 0}, {0, 1}, {0, 2}}};
    printVertexList(vl);

    EdgeList el = {6, {{{0, 0}, {1, 0}}, {{0, 1}, {1, 0}}, {{0, 1}, {1, 1}}, {{0, 2}, {1, 1}}, {{0, 0}, {1, 2}}, {{0, 2}, {1, 2}}}};

    Graph g = { .el = el, .vl = vl};

    for (int i = 0; i < g.el.len; ++i)
    {
        printEdge(g.el.edges[i]);
    }

    // getParents({1, 1}, g);
    printVertexList(getParents((Vertex) {1, 1}, g));

    return 0;
}

/* function definition */
// https://stackoverflow.com/questions/2249731/how-do-i-get-bit-by-bit-data-from-an-integer-value-in-c
// void printIntToCharConversionTable( ) {
//     char c;
//     for (c = 0; (uint) c < 256; ++c)
//     {
//         printf("%u%u%u%u%u%u%u%u, %c\n", \
//             (c & ( 1 << 7 )) >> 7, \
//             (c & ( 1 << 6 )) >> 6, \
//             (c & ( 1 << 5 )) >> 5, \
//             (c & ( 1 << 4 )) >> 4, \
//             (c & ( 1 << 3 )) >> 3, \
//             (c & ( 1 << 2 )) >> 2, \
//             (c & ( 1 << 1 )) >> 1, \
//             (c & ( 1 << 0 )) >> 0, \
//             c \
//         );
//     }
// }

// https://www.quora.com/How-do-I-convert-character-value-to-integer-value-in-c-language
char vertexToChar( Vertex v ) {
    char c;
    if (v.visible) {
        c = 'a' + v.label;
    } else {
        c = 'z' - v.label;
    }
    return c;
}

bool vertexEqualQ( Vertex v1, Vertex v2 ) {
    return (v1.visible == v2.visible && v1.label == v2.label);
}

void printEdge( Edge edge ) {
    printf("%c -> %c\n", vertexToChar(edge.parent), vertexToChar(edge.child));
}

void printVertexList( VertexList vertexList ) {
    if (vertexList.len > 0) {
        printf("%c", vertexToChar(vertexList.vertices[0]));
        for (int i = 1; i < vertexList.len; ++i)
        {
            printf(", %c", vertexToChar(vertexList.vertices[i]));
        }
        printf("\n");
    }
}

VertexList getParents(Vertex var, Graph g) {

    VertexList vl;

    vl.len = 0; // Initialize the length/index to 0

    for (int i = 0; i < g.el.len; ++i)
    {
        if ( vertexEqualQ(g.el.edges[i].child, var) ) {
            vl.vertices[vl.len] = g.el.edges[i].parent;
            vl.len += 1;
        }
    }
    return vl;
}

