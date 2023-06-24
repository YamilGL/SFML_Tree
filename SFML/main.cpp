#include <SFML/Graphics.hpp>
#include <iostream>

class Node {
public:
    int key;
    Node* left;
    Node* right;
    int height;

    Node(int key) : key(key), left(nullptr), right(nullptr), height(1) {}
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalanceFactor(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insertNode(Node* node, int key) {
    if (node == nullptr)
        return new Node(key);

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (key < node->left->key)
            return rightRotate(node);
        else if (key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }

    if (balanceFactor < -1) {
        if (key > node->right->key)
            return leftRotate(node);
        else if (key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

Node* nodeWithMinimumValue(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;
            delete temp;
        }
        else {
            Node* temp = nodeWithMinimumValue(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balanceFactor = getBalanceFactor(root);

    if (balanceFactor > 1) {
        if (getBalanceFactor(root->left) >= 0)
            return rightRotate(root);
        else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }

    if (balanceFactor < -1) {
        if (getBalanceFactor(root->right) <= 0)
            return leftRotate(root);
        else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }

    return root;
}

void drawTree(sf::RenderWindow& window, Node* root, int x, int y, int xOffset) {
    if (root == nullptr)
        return;

    int radius = 25; // Radio de los nodos del árbol
    int heightOffset = 80; // Espacio vertical entre los niveles del árbol

    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Black);
    circle.setPosition(x - radius, y - radius);

    sf::Font font;
    font.loadFromFile("arial_narrow_7.ttf");

    sf::Text text(std::to_string(root->key), font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x - text.getGlobalBounds().width / 2, y - text.getGlobalBounds().height / 2);

    window.draw(circle);
    window.draw(text);

    if (root->left != nullptr) {
        int leftChildX = x - xOffset;
        int leftChildY = y + heightOffset;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y + radius), sf::Color::Black),
            sf::Vertex(sf::Vector2f(leftChildX, leftChildY - radius), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawTree(window, root->left, leftChildX, leftChildY, xOffset / 2);
    }

    if (root->right != nullptr) {
        int rightChildX = x + xOffset;
        int rightChildY = y + heightOffset;
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y + radius), sf::Color::Black),
            sf::Vertex(sf::Vector2f(rightChildX, rightChildY - radius), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
        drawTree(window, root->right, rightChildX, rightChildY, xOffset / 2);
    }
}


int main() {
    Node* root = nullptr;
    sf::RenderWindow window(sf::VideoMode(1000, 800), "AVL Tree Visualization");

    sf::Font font;
    font.loadFromFile("arial_narrow_7.ttf");

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(24);
    instructions.setFillColor(sf::Color::Black);
    instructions.setPosition(10, 10);
    instructions.setString("Press 'I' to insert a node\nPress 'D' to delete a node");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::I) {
                    int key;
                    std::cout << "Enter the value to insert: ";
                    std::cin >> key;
                    root = insertNode(root, key);
                }
                else if (event.key.code == sf::Keyboard::D) {
                    int key;
                    std::cout << "Enter the value to delete: ";
                    std::cin >> key;
                    root = deleteNode(root, key);
                }
            }
        }

        window.clear(sf::Color::White);

        // Draw the tree
        if (root != nullptr) {
            drawTree(window, root, window.getSize().x / 2, 50, 200);
        }

        window.draw(instructions);
        window.display();
    }

    return 0;
}




