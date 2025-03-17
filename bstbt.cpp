#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
#include <cstdlib>

using namespace std;

int getValidatedInt(const string& prompt)
{
    int x;
    while (true)
    {
        cout << prompt;
        if (cin >> x)
        {
            return x;
        }
        else
        {
            cout << "Invalid input, please enter an integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ==================== Binary Search Tree (BST) Implementation ====================

class BSTNode
{
public:
    int data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BST
{
private:
    BSTNode* root;
    bool allowDuplicates;

    BSTNode* insertRecursive(BSTNode* node, int value)
    {
        if (node == nullptr)
        {
            return new BSTNode(value);
        }
        if (value < node->data)
        {
            node->left = insertRecursive(node->left, value);
        }
        else if (value > node->data)
        {
            node->right = insertRecursive(node->right, value);
        }
        else
        {
            if (allowDuplicates)
            {
                node->right = insertRecursive(node->right, value);
            }
            else
            {
                cout << "Duplicate value " << value << " is not allowed.\n";
            }
        }
        return node;
    }

    BSTNode* deleteRecursive(BSTNode* node, int value, bool& found)
    {
        if (node == nullptr)
        {
            return node;
        }
        if (value < node->data)
        {
            node->left = deleteRecursive(node->left, value, found);
        }
        else if (value > node->data)
        {
            node->right = deleteRecursive(node->right, value, found);
        }
        else
        {
            found = true;
            if (node->left == nullptr)
            {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            BSTNode* temp = node->right;
            while (temp->left != nullptr)
                temp = temp->left;
            node->data = temp->data;
            node->right = deleteRecursive(node->right, temp->data, found);
        }
        return node;
    }

    int getHeight(BSTNode* node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    bool isRightSkewed(BSTNode* node)
    {
        while (node != nullptr)
        {
            if (node->left != nullptr)
                return false;
            node = node->right;
        }
        return true;
    }

    void displayHelperBalanced()
    {
        int maxLevel = getHeight(root);
        if (maxLevel == 0)
        {
            cout << "Tree is empty.\n";
            return;
        }
        int spacingFactor = 2; // You can adjust this factor as needed
        vector<BSTNode*> current;
        current.push_back(root);
        int level = 1;
        while (level <= maxLevel)
        {
            int initialSpaces = max(0, (((1 << (maxLevel - level)) - 1)) / spacingFactor);
            int betweenSpaces = max(0, (((1 << (maxLevel - level + 1)) - 1)) / spacingFactor);
            cout << string(initialSpaces, ' ');
            vector<BSTNode*> next;
            for (size_t i = 0; i < current.size(); i++)
            {
                if (current[i] != nullptr)
                {
                    cout << current[i]->data;
                    next.push_back(current[i]->left);
                    next.push_back(current[i]->right);
                }
                else
                {
                    cout << " ";
                    next.push_back(nullptr);
                    next.push_back(nullptr);
                }
                if (i < current.size() - 1)
                {
                    cout << string(betweenSpaces, ' ');
                }
            }
            cout << "\n";
            if (level < maxLevel)
            {
                int safeInitialSpaces = (initialSpaces > 0 ? initialSpaces - 1 : 0);
                int safeBetweenSpaces = (betweenSpaces > 2 ? betweenSpaces - 2 : 0);
                cout << string(safeInitialSpaces, ' ');
                for (size_t i = 0; i < current.size(); i++)
                {
                    if (current[i] != nullptr && current[i]->left != nullptr)
                        cout << "/";
                    else
                        cout << " ";
                    cout << string(2, ' ');
                    if (current[i] != nullptr && current[i]->right != nullptr)
                        cout << "\\";
                    else
                        cout << " ";
                    if (i < current.size() - 1)
                        cout << string(safeBetweenSpaces, ' ');
                }
                cout << "\n";
            }
            current = next;
            level++;
        }
    }

    void displaySimple()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty.\n";
            return;
        }
        queue<pair<BSTNode*, int>> q;
        q.push({ root, 1 });
        int currentLevel = 1;
        cout << "Level " << currentLevel << ": ";
        while (!q.empty())
        {
            auto p = q.front();
            q.pop();
            BSTNode* node = p.first;
            int level = p.second;
            if (level != currentLevel)
            {
                cout << "\nLevel " << level << ": ";
                currentLevel = level;
            }
            cout << node->data << " ";
            if (node->left)
                q.push({ node->left, level + 1 });
            if (node->right)
                q.push({ node->right, level + 1 });
        }
        cout << "\n";
    }

    void displayBoth()
    {
        cout << "\n--- Balanced Display ---\n";
        displayHelperBalanced();
        cout << "\n--- Simple Level Order Display ---\n";
        displaySimple();
    }


public:
    BST(bool allowDuplicates = false) : root(nullptr), allowDuplicates(allowDuplicates) {}

    void insert(int value)
    {
        root = insertRecursive(root, value);
    }

    void remove(int value)
    {
        bool found = false;
        root = deleteRecursive(root, value, found);
        if (!found)
        {
            cout << "Value " << value << " not found in the tree.\n";
        }
    }

    void display()
    {
        cout << "\nBinary Search Tree Visual Representation:\n";
        displayBoth();
    }


    void levelOrderTraversal()
    {
        vector<int> levels;
        if (root == nullptr)
        {
            cout << "Tree is empty.\n";
            return;
        }
        queue<BSTNode*> q;
        q.push(root);
        while (!q.empty())
        {
            BSTNode* curr = q.front();
            q.pop();
            levels.push_back(curr->data);
            if (curr->left != nullptr)
            {
                q.push(curr->left);
            }
            if (curr->right != nullptr)
            {
                q.push(curr->right);
            }
        }
        cout << "\nLevel Order Traversal: [";
        for (size_t i = 0; i < levels.size(); i++)
        {
            cout << levels[i];
            if (i != levels.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << "]\n";
    }

    int levelOfMax()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty.\n";
            return -1;
        }
        int level = 1;
        BSTNode* curr = root;
        while (curr->right != nullptr)
        {
            curr = curr->right;
            level++;
        }
        return level;
    }

    void showMaxElementAndLevel()
    {
        if (root == nullptr)
        {
            cout << "Tree is empty.\n";
            return;
        }
        int level = 1;
        BSTNode* curr = root;
        while (curr->right != nullptr)
        {
            curr = curr->right;
            level++;
        }
        cout << "Maximum Element: " << curr->data << ", at Level: " << level << "\n";
    }
};

// ==================== Binary Tree Implementation ====================

class TreeNode
{
public:
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree
{
private:
    TreeNode* root;
    vector<int> insertionOrder;

    void clear(TreeNode* node)
    {
        if (node)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    int getHeight(TreeNode* node)
    {
        if (node == nullptr)
            return 0;
        {
            return 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    TreeNode* balancedAdd(TreeNode* subtree, int value)
    {
        if (subtree == nullptr)
        {
            return new TreeNode(value);
        }
        int leftHeight = getHeight(subtree->left);
        int rightHeight = getHeight(subtree->right);
        if (leftHeight <= rightHeight)
        {
            subtree->left = balancedAdd(subtree->left, value);
        }
        else
        {
            subtree->right = balancedAdd(subtree->right, value);
        }
        return subtree;
    }

    TreeNode* removeNode(TreeNode* node, int value, bool& removed)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        if (node->value == value && node->left == nullptr && node->right == nullptr)
        {
            removed = true;
            delete node;
            return nullptr;
        }
        node->left = removeNode(node->left, value, removed);
        if (!removed)
        {
            node->right = removeNode(node->right, value, removed);
        }
        return node;
    }

    vector<string> buildTreeString(TreeNode* node)
    {
        vector<string> result;
        if (node == nullptr)
        {
            return result;
        }
        string nodeStr = to_string(node->value);
        vector<string> leftLines = buildTreeString(node->left);
        vector<string> rightLines = buildTreeString(node->right);
        int leftWidth = leftLines.empty() ? 0 : leftLines[0].size(); // condition ? expression_if_true : expression_if_false;
        int rightWidth = rightLines.empty() ? 0 : rightLines[0].size();
        string firstLine = string(leftWidth, ' ') + nodeStr + string(rightWidth, ' ');
        string secondLine;
        if (node->left)
        {
            secondLine += string(max(0, leftWidth - 1), ' ') + "/";
        }
        else
        {
            secondLine += string(leftWidth, ' ');
        }
        secondLine += string(nodeStr.size(), ' ');
        if (node->right)
        {
            secondLine += "\\" + string(max(0, rightWidth - 1), ' ');
        }
        else
        {
            secondLine += string(rightWidth, ' ');
        }
        int maxSubLines = max(leftLines.size(), rightLines.size());
        vector<string> mergedSubLines;
        for (int i = 0; i < maxSubLines; i++)
        {
            string leftLine = (i < leftLines.size()) ? leftLines[i] : string(leftWidth, ' ');
            string rightLine = (i < rightLines.size()) ? rightLines[i] : string(rightWidth, ' ');
            mergedSubLines.push_back(leftLine + string(nodeStr.size(), ' ') + rightLine);
        }
        result.push_back(firstLine);
        if (node->left || node->right)
        {
            result.push_back(secondLine);
        }
        result.insert(result.end(), mergedSubLines.begin(), mergedSubLines.end());
        return result;
    }

    bool isFullHelper(TreeNode* node)
    {
        if (node == nullptr)
        {
            return true;
        }
        if (node->left == nullptr && node->right == nullptr)
        {
            return true;
        }
        if (node->left && node->right)
        {
            return isFullHelper(node->left) && isFullHelper(node->right);
        }
        return false;
    }

    int findDepth(TreeNode* node)
    {
        int d = 0;
        while (node != nullptr)
        {
            d++;
            node = node->left;
        }
        return d;
    }

    bool isPerfectHelper(TreeNode* node, int depth, int level)
    {
        if (node == nullptr)
        {
            return true;
        }
        if (node->left == nullptr && node->right == nullptr)
        {
            return (depth == level + 1);
        }
        if (node->left == nullptr || node->right == nullptr)
        {
            return false;
        }
        return isPerfectHelper(node->left, depth, level + 1) && isPerfectHelper(node->right, depth, level + 1);
    }

    bool existsHelper(TreeNode* node, int value)
    {
        if (node == nullptr)
        {
            return false;
        }
        if (node->value == value)
        {
            return true;
        }
        return existsHelper(node->left, value) || existsHelper(node->right, value);
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree()
    {
        clear(root);
    }

    void insert(int value)
    {
        root = balancedAdd(root, value);
        insertionOrder.push_back(value);
    }

    bool exists(int value)
    {
        return existsHelper(root, value);
    }

    bool removeLast()
    {
        if (insertionOrder.empty())
        {
            return false;
        }
        int lastValue = insertionOrder.back();
        insertionOrder.pop_back();
        bool removed = false;
        root = removeNode(root, lastValue, removed);
        return removed;
    }

    void display()
    {
        cout << "\nTree Visualization:\n";
        vector<string> lines = buildTreeString(root);
        for (const auto& line : lines)
        {
            cout << line << "\n";
        }
    }

    bool isFull()
    {
        return isFullHelper(root);
    }

    bool isComplete()
    {
        if (root == nullptr)
        {
            return true;
        }
        queue<TreeNode*> q;
        q.push(root);
        bool encounteredNull = false;
        while (!q.empty())
        {
            TreeNode* cur = q.front();
            q.pop();
            if (cur == nullptr)
            {
                encounteredNull = true;
            }
            else
            {
                if (encounteredNull)
                {
                    return false;
                }
                q.push(cur->left);
                q.push(cur->right);
            }
        }
        return true;
    }

    bool isPerfect()
    {
        int depth = findDepth(root);
        return isPerfectHelper(root, depth, 0);
    }
};

// ==================== Persistent Menu Functions ====================

void runBST(BST* bst)
{
    while (true)
    {
        clearScreen();
        cout << "\nBinary Search Tree Menu:\n";
        cout << "1. Insertion\n";
        cout << "2. Deletion\n";
        cout << "3. Display (Both Formats)\n";
        cout << "4. Level Order Traversal (BFS)\n";
        cout << "5. Show Maximum Element and its Level\n";
        cout << "6. Exit to Main Menu\n";
        int choice = getValidatedInt("Enter your choice: ");
        if (choice == 1)
        {
            int value = getValidatedInt("Enter integer to insert: ");
            bst->insert(value);
            cout << value << " has been added to BST.\n";
            waitForEnter();
        }
        else if (choice == 2)
        {
            int value = getValidatedInt("Enter integer to delete: ");
            bst->remove(value);
            waitForEnter();
        }
        else if (choice == 3)
        {
            bst->display();
            waitForEnter();
        }
        else if (choice == 4)
        {
            bst->levelOrderTraversal();
            waitForEnter();
        }
        else if (choice == 5)
        {
            bst->showMaxElementAndLevel();
            waitForEnter();
        }
        else if (choice == 6)
        {
            break;
        }
        else
        {
            cout << "Invalid choice, Please try again.\n";
            waitForEnter();
        }
    }
}

void runBinaryTree(BinaryTree* tree, bool duplicatesAllowed)
{
    while (true)
    {
        clearScreen();
        cout << "\nBinary Tree Menu:\n";
        cout << "1. Insert new integer\n";
        cout << "2. Delete last inserted element\n";
        cout << "3. Display tree\n";
        cout << "4. Check tree properties (Full, Complete, Perfect)\n";
        cout << "5. Exit to Main Menu\n";
        int choice = getValidatedInt("Enter your choice: ");
        if (choice == 1)
        {
            int val = getValidatedInt("Enter integer to insert: ");
            if (!duplicatesAllowed && tree->exists(val))
            {
                cout << "Duplicates are not allowed. Value " << val << " already exists.\n";
            }
            else
            {
                tree->insert(val);
                cout << "Inserted " << val << " into the tree.\n";
            }
            waitForEnter();
        }
        else if (choice == 2)
        {
            if (tree->removeLast())
            {
                cout << "Last inserted element removed.\n";
            }
            else
            {
                cout << "Tree is empty, nothing to remove.\n";
            }
            waitForEnter();
        }
        else if (choice == 3)
        {
            tree->display();
            waitForEnter();
        }
        else if (choice == 4)
        {
            cout << "\nTree Properties:\n";
            cout << "Full Binary Tree: " << (tree->isFull() ? "Yes" : "No") << "\n";
            cout << "Complete Binary Tree: " << (tree->isComplete() ? "Yes" : "No") << "\n";
            cout << "Perfect Binary Tree: " << (tree->isPerfect() ? "Yes" : "No") << "\n";
            waitForEnter();
        }
        else if (choice == 5)
        {
            break;
        }
        else
        {
            cout << "Invalid choice, please try again.\n";
            waitForEnter();
        }
    }
}

// ==================== Main Function ====================

int main()
{
    BST* bst = nullptr;
    BinaryTree* btree = nullptr;
    bool btDuplicatesAllowed = true; // default; will be set when first created
    while (true)
    {
        clearScreen();
        cout << "\nMain Menu:\n";
        cout << "1. Binary Search Tree (BST)\n";
        cout << "2. Binary Tree\n";
        cout << "3. Exit\n";
        int mainChoice = getValidatedInt("Enter your choice: ");
        if (mainChoice == 1)
        {
            if (bst == nullptr)
            {
                int allowDup = getValidatedInt("Do you want to allow duplicates in the Binary Search Tree? (Enter 1 for Yes, 0 for No): ");
                bst = new BST(allowDup == 1);
            }
            runBST(bst);
        }
        else if (mainChoice == 2)
        {
            if (btree == nullptr)
            {
                int allowDup = getValidatedInt("Do you want to allow duplicates in the Binary Tree? (Enter 1 for Yes, 0 for No): ");
                btDuplicatesAllowed = (allowDup == 1);
                btree = new BinaryTree();
            }
            runBinaryTree(btree, btDuplicatesAllowed);
        }
        else if (mainChoice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice, Please try again.\n";
            waitForEnter();
        }
    }
    delete bst;
    delete btree;
    return 0;
}
