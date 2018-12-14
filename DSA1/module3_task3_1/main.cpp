// 3_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе
// приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода
// должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
// За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты
// в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять
// солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он
// должен становится. Требуемая скорость выполнения команды - O(log n).

// Формат входных данных.
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой следующей строке содержится описание команды: число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.

// Формат выходных данных.
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
// на которую должен встать этот солдат (все стоящие за ним двигаются назад).


#include <iostream>
#include <fstream>
using namespace std;

// описание узла АВЛ-дерева
struct AVLTree {
    AVLTree* left;
    AVLTree* right;
    int height;
    int key;
    // количество детей узла + 1
    int children;
    AVLTree( int value ) { left = NULL; right = NULL; height = 1; key = value; children = 1; }
};

class CTree {

public:
    void HeightOfSoldier( AVLTree* node, int position, int &height );
    void PositionOfSoldier( AVLTree* node, int height, int &counter );
    AVLTree* AVLTreeInsert( AVLTree* node, int value );
    AVLTree* AVLTreeDelete( AVLTree* node, int value );
    void DFSWrite( AVLTree* node, int counter );
    void DeleteTree( AVLTree* node );
    CTree( AVLTree* node );
    ~CTree();

private:
    AVLTree* root;
    int HeightOf( AVLTree* node );
    int BalanceOf( AVLTree* node );
    void BalanceChildren( AVLTree*& node );
    void NewHeight( AVLTree*& node );
    AVLTree* LeftRotate( AVLTree* node );
    AVLTree* RightRotate( AVLTree* node );
    AVLTree* Balancing( AVLTree* node );
    AVLTree* AVLTreeMin( AVLTree* node );
    AVLTree* AVLTreeDeleteMin( AVLTree* node );
};

CTree::CTree( AVLTree* node )
{
    root = node;
}

CTree::~CTree()
{
    DeleteTree( root );
}

// возвращает высоту поддерева для конкретного узла
int CTree::HeightOf( AVLTree* node )
{
    if( node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}

// возвращает баланс для конкретного узла
int CTree::BalanceOf( AVLTree* node )
{
    // баланс - разность высот правого и левого поддеревьев
    return ( HeightOf(node->right) - HeightOf(node->left) );
}

// обновляет количество детей для конкретного узла
void CTree::BalanceChildren( AVLTree*& node )
{
    if( node == NULL ) {
        return;
    } else {
        node->children = 1;
        if( node->left != NULL ) {
            node->children += node->left->children;
        }
        if( node->right != NULL ) {
            node->children += node->right->children;
        }
    }
}

// восстанавливает корректное значение height для конкретного узла
void CTree::NewHeight( AVLTree*& node )
{
    // выбирается максимальная высота из двух поддеревьев + 1
    int HeightL = HeightOf( node->left );
    int HeightR = HeightOf( node->right );
    if( HeightL > HeightR ) {
        node->height = HeightL + 1;
    } else {
        node->height = HeightR + 1;
    }
}

// малое левое вращение
AVLTree* CTree::LeftRotate( AVLTree* node )
{
    AVLTree* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    NewHeight( node );
    BalanceChildren( node );
    NewHeight( tmp );
    BalanceChildren( tmp );
    return tmp;
}

// малое правое вращение
AVLTree* CTree::RightRotate( AVLTree* node )
{
    AVLTree* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    NewHeight( node );
    BalanceChildren( node );
    NewHeight( tmp );
    BalanceChildren( tmp );
    return tmp;
}

// балансировка
AVLTree* CTree::Balancing( AVLTree* node )
{
    NewHeight( node );
    BalanceChildren( node );
    if( BalanceOf( node ) == -2 ) {
        int BalanceOfLeft;
        if( node->left == NULL ) {
            BalanceOfLeft = 0;
        } else {
            BalanceOfLeft = BalanceOf( node->left );
        }
        if( BalanceOfLeft > 0 ) {
            node->left = LeftRotate( node->left );
        }
        return RightRotate( node );
    }
    if( BalanceOf( node ) == 2 ) {
        int BalanceOfRight;
        if( node->right == NULL ) {
            BalanceOfRight = 0;
        } else {
            BalanceOfRight = BalanceOf( node->right );
        }
        if( BalanceOfRight < 0 ) {
            node->right = RightRotate( node->right );
        }
        return LeftRotate( node );
    }
    return node;
}

// вставка узла в АВЛ-дерево
AVLTree* CTree::AVLTreeInsert( AVLTree* node, int value )
{
    if( node == NULL ) {
        node = new AVLTree( value );
        return node;
    }
    if( value <= node->key ) {
        node->left = AVLTreeInsert( node->left, value );
    } else {
        node->right = AVLTreeInsert( node->right, value );
    }

    return Balancing( node );
}

// рекурсивный спуск до минимального узла дерева
AVLTree* CTree::AVLTreeMin( AVLTree* node )
{
    if( node->left == NULL ) {
        return node;
    } else {
        return AVLTreeMin( node-> left );
    }
}

// удаление минимального узла дерева
AVLTree* CTree::AVLTreeDeleteMin( AVLTree* node )
{
    if( node->left == NULL ) {
        return node->right;
    }
    node->left = AVLTreeDeleteMin( node->left );
    return Balancing( node );
}

// удаление узла из АВЛ-дерева
AVLTree* CTree::AVLTreeDelete( AVLTree* node, int value )
{
    if( node == NULL ) {
        return NULL;
    }
    if( value == node->key ) {
        AVLTree* nodeLeft = node->left;
        AVLTree* nodeRight = node->right;
        delete node;
        if( nodeRight == NULL ) {
            return nodeLeft;
        } else {
            AVLTree* minn = AVLTreeMin( nodeRight );
            minn->right = AVLTreeDeleteMin( nodeRight );
            minn->left = nodeLeft;
            return Balancing( minn );
        }
    }

    if( value < node->key ) {
        node->left = AVLTreeDelete( node->left, value );
    }
    if( value > node->key ) {
        node->right = AVLTreeDelete( node->right, value );
    }
    return Balancing( node );
}

// нахождение позиции солдата по его росту
void CTree::PositionOfSoldier( AVLTree* node, int height, int& counter )
{
    if( height == node->key ) {
        if( node->right != NULL ) {
            counter = counter + node->right->children;
        }
    }
    if( height > node->key ) {
        PositionOfSoldier( node->right, height, counter );
    }
    if( height < node->key ) {
        if( node->right != NULL ) {
            counter = counter + node->right->children + 1;
            PositionOfSoldier( node->left, height, counter );
        } else {
            counter = counter + 1;
            PositionOfSoldier( node->left, height, counter );
        }
    }
}

// нахождение роста солдата по его позиции
void CTree::HeightOfSoldier( AVLTree* node, int position, int& height )
{
    int RootRightChildren = 0;
    if( node->right != NULL) {
        RootRightChildren = node->right->children;
    }
    if( position == RootRightChildren ) {
        height = node->key;
    }
    if( position > RootRightChildren ) {
        HeightOfSoldier( node->left, position - RootRightChildren - 1, height );
    }
    if( position < RootRightChildren ) {
        HeightOfSoldier( node->right, position, height );
    }
}

// удаление дерева
void CTree::DeleteTree( AVLTree* node ) {
    if( node == NULL ) {
        return;
    }
    DeleteTree( node->left );
    DeleteTree( node->right );
    delete node;
}

// вывод дерева в порядке сверху вниз
void CTree::DFSWrite( AVLTree* root, int counter )
{
    if( root == NULL ) {
        return;
    }
    cout << root->key << " " << counter << endl;
    DFSWrite( root->left, counter + 1 );
    DFSWrite( root->right, counter + 1 );
}

int main() {

    // количество операций
    int n;
    cin >> n;

    // корень дерева
    int operation;
    AVLTree* node = NULL;
    CTree root( node );

    // считывание солдат и построение дерева
    for( int i = 0; i < n; i++ ) {
        cin >> operation;
        if( operation == 1 ) {
            int height;
            cin >> height;
            node = root.AVLTreeInsert( node, height );
            int position = 0;
            root.PositionOfSoldier( node, height, position );
            cout << position << endl;
        }
        if( operation == 2 ) {
            int position;
            cin >> position;
            int height;
            root.HeightOfSoldier( node, position, height );
            node = root.AVLTreeDelete( node, height );
        }
        // DFSWrite(node, 0);
    }

    root.DeleteTree( node );
}
