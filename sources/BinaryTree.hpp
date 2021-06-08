#include <iostream>
#include <iterator>
#include <stack>
#include <deque>

namespace ariel
{
    template <typename T>
    class BinaryTree
    {
        class Node
        {
            T value;
            Node *left, *right;

        public:
            //Node Constructor
            Node(T addValue)
                : value(addValue), left(nullptr), right(nullptr){};
            //Node deep copy constructor
            Node(const Node *other) : value(other->value), left(nullptr), right(nullptr)
            {
                if (other)
                {
                    if (other->left)
                    {
                        left = new Node(other->left);
                    }
                    if (other->right)
                    {
                        right = new Node(other->right);
                    }
                }
            }

            bool operator==(const Node &other)
            {
                return value == other.value && left == other.left && right == other.right;
            }

            friend class BinaryTree;
        };

        enum Order
        {
            preorder,
            inorder,
            postorder
        };

        Node *root;

    public:
        class iterator
        {
            template <Node *, class Container = std::deque<Node *>>
            class stack;

            Node *current;
            Order order;
            std::stack<Node *> st;

        public:
            iterator(Node *start, const Order &orderBy) : order(orderBy)
            {
                //init stack
                if (start)
                {
                    initStack(start);
                }
                //check that stack is not empty
                if (st.empty())
                {
                    current = nullptr;
                }
                else
                {
                    current = st.top();

                    st.pop();
                }
            }
            iterator() : current(nullptr){};

            void initStack(Node *n)
            {
                if (n == nullptr)
                {
                    return;
                }
                //backwards for stack
                if (order == preorder)
                {
                    initStack(n->right);
                    initStack(n->left);
                    st.push(n);
                }
                else if (order == inorder)
                {
                    initStack(n->right);
                    st.push(n);
                    initStack(n->left);
                }
                else
                {
                    st.push(n);
                    initStack(n->right);
                    initStack(n->left);
                }
            }

            iterator &operator++()
            {
                if (st.empty())
                {
                    current = nullptr;
                    return *this;
                }
                current = st.top();
                st.pop();
                return *this;
            }

            iterator operator++(int)
            {
                iterator temp;
                temp.current = new Node(current);
                if (!st.empty())
                {
                    current = st.top();
                    st.pop();
                }
                else
                {
                    current = nullptr;
                }

                return temp;
            }

            T &operator*() const
            {
                return current->value;
            }

            T *operator->() const
            {
                return &(current->value);
            }

            bool operator==(const iterator &it) const
            {
                if (current == nullptr || it.current == nullptr)
                {
                    return (current == nullptr && it.current == nullptr);
                }

                return current->value == it.current->value;
            }

            bool operator!=(const iterator &it) const
            {
                return !(*this == it);
            }
        };

        BinaryTree<T>() : root(nullptr) {}

        ~BinaryTree<T>()
        {
            clean(root);
        }

        void clean(Node *n)
        {
            if (n == nullptr)
            {
                return;
            }
            clean(n->left);
            clean(n->right);
            delete n;
        }

        BinaryTree<T>(const BinaryTree<T> &other) : root(nullptr)
        {
            if (other.root)
            {
                root = new Node(other.root);
            }
        }

        BinaryTree<T>(BinaryTree<T> &&other) noexcept : root(other.root)
        {
            other.root = nullptr;
        }

        BinaryTree<T> &operator=(const BinaryTree<T> &other)
        {
            if (this == &other)
            {
                return *this;
            }
            if (root)
            {
                delete root;
            }
            if (other.root)
            {
                root = new Node(other.root);
            }
            return *this;
        }

        BinaryTree<T> &operator=(BinaryTree<T> &&other) noexcept
        {
            if (root)
            {
                delete root;
            }
            root = other.root;
            other.root = nullptr;

            return *this;
        }

        bool operator==(const BinaryTree<T> &other)
        {
            return root == other.root;
        }

        BinaryTree<T> &add_root(const T &value)
        {
            if (root)
            {
                root->value = value;
            }
            else
            {
                root = new Node(value);
            }

            return *this;
        }

        BinaryTree<T> &add_left(const T &exist, const T &left_child)
        {
            Node *node = searchNode(root, exist);

            if (node != nullptr)
            {
                if (node->left == nullptr)
                {
                    node->left = new Node(left_child);
                }
                else
                {
                    node->left->value = left_child;
                }

                return *this;
            }
            throw std::invalid_argument("No node with that value!");
        }

        BinaryTree<T> &add_right(const T &exist, const T &right_child)
        {
            Node *node = searchNode(root, exist);
            if (node != nullptr)
            {
                if (node->right == nullptr)
                {
                    (node->right) = new Node(right_child);
                }
                else
                {
                    node->right->value = right_child;
                }
                return *this;
            }
            throw std::invalid_argument("No node with that value!");
        }

        Node *searchNode(Node *currentNode, const T &toFind)
        {
            if (currentNode == nullptr)
            {
                return nullptr;
            }
            if (currentNode->value == toFind)
            {
                return currentNode;
            }

            Node *right = searchNode(currentNode->right, toFind);
            Node *left = searchNode(currentNode->left, toFind);
            if (right != nullptr)
            {
                return right;
            }
            return left;
        }

        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree) { return os; }

        iterator begin_preorder() { return iterator{root, preorder}; }

        iterator end_preorder() { return iterator{}; }

        iterator begin_inorder() { return iterator{root, inorder}; }

        iterator end_inorder() { return iterator{}; }

        iterator begin_postorder() { return iterator{root, postorder}; }

        iterator end_postorder() { return iterator{}; }

        iterator begin() { return iterator{root, inorder}; }

        iterator end() { return iterator{}; }
    };
}