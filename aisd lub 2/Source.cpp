#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T>
struct Node {
    T data;
    Node<T>* prev;
    Node<T>* next;

    Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    LinkedList(const LinkedList& other) {
        head = nullptr;
        tail = nullptr;

        Node<T>* curr = other.head;
        while (curr != nullptr) {
            push_tail(curr->data);
            curr = curr->next;
        }
    }

    LinkedList(const T& data) : head(nullptr), tail(nullptr) {
        push_tail(data);
    }

    ~LinkedList() {
        clear();
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) {
            return *this;
        }

        clear();

        Node<T>* curr = other.head;
        while (curr != nullptr) {
            push_tail(curr->data);
            curr = curr->next;
        }

        return *this;
    }

    void push_tail(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (tail != nullptr) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else {
            head = newNode;
            tail = newNode;
        }
    }

    void push_tail(const LinkedList& other) {
        Node<T>* curr = other.head;
        while (curr != nullptr) {
            push_tail(curr->data);
            curr = curr->next;
        }
    }

    void push_head(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (head != nullptr) {
            head->prev = newNode;
            newNode->next = head;
            head = newNode;
        }
        else {
            head = newNode;
            tail = newNode;
        }
    }

    void push_head(const LinkedList& other) {
        LinkedList reversed = other;
        reversed.reverse();
        push_tail(reversed);
    }

    void pop_head() {
        if (head != nullptr) {
            Node<T>* temp = head;
            head = temp->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            else {
                tail = nullptr;
            }
            delete temp;
        }
    }

    void pop_tail() {
        if (tail != nullptr) {
            Node<T>* temp = tail;
            tail = temp->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            }
            else {
                head = nullptr;
            }
            delete temp;
        }
    }

    void delete_node(const T& data) {
        Node<T>* curr = head;
        while (curr != nullptr) {
            if (curr->data == data) {
                if (curr == head) {
                    pop_head();
                }
                else if (curr == tail) {
                    pop_tail();
                }
                else {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                    delete curr;
                }
                break;
            }
            curr = curr->next;
        }
    }

    T& operator[](int index) {
        Node<T>* curr = head;
        for (int i = 0; i < index && curr != nullptr; i++) {
            curr = curr->next;
        }
        if (curr == nullptr) {
            // Выбросить исключение или вернуть значение по умолчанию
            throw out_of_range("Index out of range");
        }
        return curr->data;
    }

    const T& operator[](int index) const {
        Node<T>* curr = head;
        for (int i = 0; i < index && curr != nullptr; i++) {
            curr = curr->next;
        }
        if (curr == nullptr) {
            // Выбросить исключение или вернуть значение по умолчанию
            throw out_of_range("Index out of range");
        }
        return curr->data;
    }

    void clear() {
        Node<T>* temp = head;
        while (temp != nullptr) {
            Node<T>* next = temp->next;
            delete temp;
            temp = next;
        }
        head = nullptr;
        tail = nullptr;
    }

    void reverse() {
        Node<T>* temp = nullptr;
        Node<T>* curr = head;
        while (curr != nullptr) {
            temp = curr->prev;
            curr->prev = curr->next;
            curr->next = temp;
            curr = curr->prev;
        }
        if (temp != nullptr) {
            head = temp->prev;
        }
    }

    int size() const {
        int count = 0;
        Node<T>* curr = head;
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
        return count;
    }
};

int main() {
    // Пример использования
    LinkedList<int> list1;
    list1.push_tail(1);
    list1.push_tail(2);
    list1.push_tail(3);

    LinkedList<int> list2(list1);

    list2.push_tail(4);
    list2.push_tail(5);

    cout << "List 1: ";
    for (int i = 0; i < list1.size(); i++) {
        cout << list1[i] << " ";
    }
    cout << endl;

    cout << "List 2: ";
    for (int i = 0; i < list2.size(); i++) {
        cout << list2[i] << " ";
    }
    cout << endl;

    return 0;
}