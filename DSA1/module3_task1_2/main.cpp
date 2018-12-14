// Дано число N < 10^6 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке pre-order (сверху вниз).

#include <iostream>
using namespace std;

struct CNode {
    int data;
    CNode* left;
    CNode* right;
    CNode ( int value ) { data = value; left = NULL; right = NULL; }
};

class CBinTree {
public:

    void Insert( CNode*& node, int value );
    void DFSWrite( CNode* node );
    void DFSDelete( CNode* node );
    CBinTree( CNode* node );
    ~CBinTree();

private:

    CNode* root;
};

CBinTree::CBinTree( CNode* node )
{
    root = node;
}

void CBinTree::Insert( CNode*& node, int value )
{
    if( node == NULL ) {
        node = new CNode( value );
        return;
    }

    if( node->data > value ) {
        Insert( node->left, value );
    } else {
        Insert( node->right, value );
    }
}

void CBinTree::DFSWrite( CNode* node )
{
    if( node == NULL ) {
        return;
    }
    cout << node->data << " ";
    DFSWrite( node->left );
    DFSWrite( node->right );
}

void CBinTree::DFSDelete( CNode* node )
{
    if( node == NULL ) {
        return;
    }
    DFSDelete( node->left );
    DFSDelete( node->right );
    delete node;
}

CBinTree::~CBinTree()
{
    DFSDelete( root );
}

int main() {
    int n;
    cin >> n;
    int value;
    cin >> value;
    CNode* node = new CNode( value );
    CBinTree root( node );
    for( int i = 0; i < n - 1 ; i++ ) {
        cin >> value;
        root.Insert( node, value );
    }

    root.DFSWrite( node );
}
