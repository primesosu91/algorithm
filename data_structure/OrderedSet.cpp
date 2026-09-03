#include <bits/stdc++.h>
using namespace std;

template<class T, class Compare = less<T>>
struct OrderedSet {
private:
    struct Node {
        T value;
        uint64_t priority;
        Node* left;
        Node* right;
        Node* parent;
        int subtree_size;

        Node(const T& value, uint64_t priority)
            : value(value), priority(priority), left(nullptr), right(nullptr),
              parent(nullptr), subtree_size(1) {}
    };

    Node* root;
    Compare comp;
    mt19937_64 rng;

    int node_size(Node* node) const {
        return node ? node->subtree_size : 0;
    }

    void update(Node* node) {
        if (node) {
            node->subtree_size = 1 + node_size(node->left) + node_size(node->right);
        }
    }

    Node* leftmost(Node* node) const {
        assert(node);

        while (node->left) {
            node = node->left;
        }

        return node;
    }

    Node* rightmost(Node* node) const {
        assert(node);

        while (node->right) {
            node = node->right;
        }

        return node;
    }

    pair<Node*, Node*> split(Node* node, const T& value) {
        if (!node) return {nullptr, nullptr};

        if (comp(node->value, value)) {
            auto [left, right] = split(node->right, value);

            node->right = left;
            if (left) left->parent = node;

            node->parent = nullptr;
            if (right) right->parent = nullptr;

            update(node);

            return {node, right};
        } else {
            auto [left, right] = split(node->left, value);

            node->left = right;
            if (right) right->parent = node;

            node->parent = nullptr;
            if (left) left->parent = nullptr;

            update(node);

            return {left, node};
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left) {
            if (right) right->parent = nullptr;
            return right;
        }

        if (!right) {
            if (left) left->parent = nullptr;
            return left;
        }

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            if (left->right) left->right->parent = left;

            left->parent = nullptr;
            update(left);

            return left;
        } else {
            right->left = merge(left, right->left);
            if (right->left) right->left->parent = right;

            right->parent = nullptr;
            update(right);

            return right;
        }
    }

    Node* insert_node(Node* node, Node* new_node) {
        if (!node) return new_node;

        if (new_node->priority > node->priority) {
            auto [left, right] = split(node, new_node->value);

            new_node->left = left;
            if (left) left->parent = new_node;

            new_node->right = right;
            if (right) right->parent = new_node;

            new_node->parent = nullptr;
            update(new_node);

            return new_node;
        }

        if (comp(new_node->value, node->value)) {
            node->left = insert_node(node->left, new_node);
            node->left->parent = node;
        } else {
            node->right = insert_node(node->right, new_node);
            node->right->parent = node;
        }

        update(node);

        return node;
    }

    Node* erase_node(Node* node, const T& value, bool& erased) {
        if (!node) return nullptr;

        if (comp(value, node->value)) {
            node->left = erase_node(node->left, value, erased);
            if (node->left) node->left->parent = node;
        } else if (comp(node->value, value)) {
            node->right = erase_node(node->right, value, erased);
            if (node->right) node->right->parent = node;
        } else {
            Node* left = node->left;
            Node* right = node->right;

            if (left) left->parent = nullptr;
            if (right) right->parent = nullptr;

            Node* merged = merge(left, right);

            delete node;
            erased = true;

            return merged;
        }

        update(node);

        return node;
    }

    void clear(Node* node) {
        if (!node) return;

        clear(node->left);
        clear(node->right);

        delete node;
    }

public:
    struct iterator {
    private:
        Node* node;
        const OrderedSet* owner;

        iterator(Node* node, const OrderedSet* owner)
            : node(node), owner(owner) {}

        friend struct OrderedSet;

    public:
        iterator()
            : node(nullptr), owner(nullptr) {}

        // 参照: *it -> 現在の要素
        const T& operator*() const {
            assert(node);
            return node->value;
        }

        // 参照: it-> -> 現在の要素へのポインタ
        const T* operator->() const {
            assert(node);
            return &node->value;
        }

        // 次へ: ++it
        iterator& operator++() {
            assert(node);

            if (node->right) {
                node = owner->leftmost(node->right);
            } else {
                Node* parent = node->parent;

                while (parent && node == parent->right) {
                    node = parent;
                    parent = parent->parent;
                }

                node = parent;
            }

            return *this;
        }

        // 次へ: it++
        iterator operator++(int) {
            iterator result = *this;
            ++(*this);
            return result;
        }

        // 前へ: --it
        iterator& operator--() {
            assert(owner);

            if (!node) {
                assert(owner->root);
                node = owner->rightmost(owner->root);
                return *this;
            }

            if (node->left) {
                node = owner->rightmost(node->left);
            } else {
                Node* parent = node->parent;

                while (parent && node == parent->left) {
                    node = parent;
                    parent = parent->parent;
                }

                assert(parent);
                node = parent;
            }

            return *this;
        }

        // 前へ: it--
        iterator operator--(int) {
            iterator result = *this;
            --(*this);
            return result;
        }

        // 等価判定
        bool operator==(const iterator& other) const {
            return node == other.node && owner == other.owner;
        }

        // 非等価判定
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // 初期化: OrderedSet(比較関数) -> 空の集合を構築
    OrderedSet(Compare comp = Compare())
        : root(nullptr), comp(comp), rng(0x123456789abcdefULL) {}

    ~OrderedSet() {
        clear(root);
    }

    OrderedSet(const OrderedSet&) = delete;
    OrderedSet& operator=(const OrderedSet&) = delete;

    // 挿入: insert(値) -> 新しく挿入した場合 true
    bool insert(const T& value) {
        if (contains(value)) return false;

        root = insert_node(root, new Node(value, rng()));
        root->parent = nullptr;

        return true;
    }

    // 削除: erase(値) -> 存在して削除した場合 true
    bool erase(const T& value) {
        bool erased = false;

        root = erase_node(root, value, erased);

        if (root) root->parent = nullptr;

        return erased;
    }

    // 存在判定: contains(値) -> 存在するなら true
    bool contains(const T& value) const {
        Node* node = root;

        while (node) {
            if (comp(value, node->value)) {
                node = node->left;
            } else if (comp(node->value, value)) {
                node = node->right;
            } else {
                return true;
            }
        }

        return false;
    }

    // 要素数: size() -> 現在の要素数
    int size() const {
        return node_size(root);
    }

    // 空判定: empty() -> 要素が存在しないなら true
    bool empty() const {
        return root == nullptr;
    }

    // k 番目: kth(k) -> 比較順で 0-indexed の k 番目の値
    const T& kth(int k) const {
        assert(0 <= k && k < size());

        Node* node = root;

        while (true) {
            int left_size = node_size(node->left);

            if (k < left_size) {
                node = node->left;
            } else if (k == left_size) {
                return node->value;
            } else {
                k -= left_size + 1;
                node = node->right;
            }
        }
    }

    // 順位: order_of(値) -> 比較順で value より前にある要素数
    int order_of(const T& value) const {
        int result = 0;
        Node* node = root;

        while (node) {
            if (comp(node->value, value)) {
                result += node_size(node->left) + 1;
                node = node->right;
            } else {
                node = node->left;
            }
        }

        return result;
    }

    // 先頭: begin() -> 比較順で最初の要素
    iterator begin() const {
        return iterator(root ? leftmost(root) : nullptr, this);
    }

    // 終端: end() -> 終端イテレータ
    iterator end() const {
        return iterator(nullptr, this);
    }

    // 検索: find(値) -> 見つからなければ end()
    iterator find(const T& value) const {
        Node* node = root;

        while (node) {
            if (comp(value, node->value)) {
                node = node->left;
            } else if (comp(node->value, value)) {
                node = node->right;
            } else {
                return iterator(node, this);
            }
        }

        return end();
    }

    // 下限: lower_bound(値) -> 比較順で value より前ではない最初の要素
    iterator lower_bound(const T& value) const {
        Node* node = root;
        Node* result = nullptr;

        while (node) {
            if (!comp(node->value, value)) {
                result = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }

        return iterator(result, this);
    }

    // 上限: upper_bound(値) -> 比較順で value より後ろにある最初の要素
    iterator upper_bound(const T& value) const {
        Node* node = root;
        Node* result = nullptr;

        while (node) {
            if (comp(value, node->value)) {
                result = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }

        return iterator(result, this);
    }
};

// examples

void yosupo_judge_Orderd_Set() {
    int N, Q;
    cin >> N >> Q;
    OrderedSet<int> st;
    for (int i = 0; i < N; i ++) {
        int a;
        cin >> a;
        st.insert(a);
    }
    for (int q = 0; q < Q; q ++) {
        int query, x;
        cin >> query >> x;
        if (query == 0) {
            st.insert(x);
        } else if (query == 1) {
            st.erase(x);
        } else if (query == 2) {
            if (x <= st.size()) {
                cout << st.kth(x - 1) << endl;
            } else {
                cout << -1 << endl;
            }
        } else if (query == 3) {
            cout << st.order_of(x + 1) << endl;
        } else if (query == 4) {
            auto it = st.upper_bound(x);
            if (it != st.begin()) {
                it --;
                cout << *it << endl;
            } else {
                cout << -1 << endl;
            }
        } else {
            auto it = st.lower_bound(x);
            if (it != st.end()) {
                cout << *it << endl;
            } else {
                cout << -1 << endl;
            }
        }
    }
}

int main() {
    yosupo_judge_Orderd_Set();
}