#include <string>
#include <iostream>

class tree_module
{
private:
    struct Node
    {
        Node *left;         // lewa gałąź, wskaźnik na node
        Node *right;        // prawa gałąź, wskaźnik na node
        int index;          // numer pakietu
        std::string packet; // string z pakietem
    };

    Node *root; // korzeń drzewa

    void insert_packet_rec(Node *&node, int index, std::string packet) { // rekursywna funkcja dodająca element do drzewa
        if (node == nullptr) {
            // Create a new node if the current node is nullptr
            node = new Node;
            node->index = index;
            node->packet = packet;
            node->left = nullptr;
            node->right = nullptr;
        } else if (index <= node->index) {
            // Recursively insert into the left subtree
            insert_packet_rec(node->left, index, packet);
        } else {
            // Recursively insert into the right subtree
            insert_packet_rec(node->right, index, packet);
        }
    }


    void travel_tree(Node *node) {                                                // funkcja poruszająca się po całym drzewie aby wyświetlić jego zawartość w dobrej kolejnosci
        if (node != nullptr) {
            // Traverse the left subtree
            travel_tree(node->left);
            // Print the current node
            std::cout << "Index: " << node->index+1 << ", Packet: " << node->packet << std::endl;
            // Traverse the right subtree
            travel_tree(node->right);
        }
    }
public:
    tree_module() : root(nullptr) {} // constructor to initialize root

    void send_packet(std::string packet, int index) { // wysyła pakiet, używa prywatnej funkcji insert_packet_rec
        insert_packet_rec(root, index, packet);
    }

    void out_complete_message() {                            // zwraca kompletną wiadomość, używa prywatnej funkcji travel_tree
        travel_tree(root);
    }

    bool clear() {
        clear_tree(root);
        return true;
    }

    // Function to clear the tree (deallocate memory)
    void clear_tree(Node* node) {
        if (node != nullptr) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    ~tree_module() {
        clear(); // call clear function to deallocate memory
    }
};
