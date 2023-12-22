#include <iostream>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <random>


using namespace std;

template <typename T>
class LinkedList {

public:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };
private:

    Node* head;
    Node* tail;

public:

    Node* get_head() const {
        return head;
    }

    Node* get_tail() const {
        return tail;
    }

    LinkedList() : head(nullptr), tail(nullptr) {}

    LinkedList(const LinkedList& other) {
        if (other.head == nullptr) {
            head = nullptr;
            tail = nullptr;
            return;
        }

        head = new Node(other.head->data);
        Node* current = head;

        Node* otherCurrent = other.head->next;
        while (otherCurrent != nullptr) {
            current->next = new Node(otherCurrent->data);
            current->next->prev = current;
            current = current->next;
            otherCurrent = otherCurrent->next;
        }

        tail = current;
    }

    LinkedList(size_t size) {
        head = nullptr;
        tail = nullptr;
        random_device rd;
        default_random_engine gen(rd());
        uniform_int_distribution<T> dis(numeric_limits<T>::min(), numeric_limits<T>::max());

        for (size_t i = 0; i < size; ++i) {
            T value = dis(gen);
            push_tail(value);
        }
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) {
            return *this;
        }

        LinkedList temp(other);
        swap(head, temp.head);
        swap(tail, temp.tail);

        return *this;
    }

    void push_head(const T& value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void push_head(const LinkedList& other) {
        Node* otherCurrent = other.tail;
        while (otherCurrent != nullptr) {
            push_head(otherCurrent->data);
            otherCurrent = otherCurrent->prev;
        }
    }

    void push_tail(const T& value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void push_tail(const LinkedList& other) {
        Node* otherCurrent = other.head;
        while (otherCurrent != nullptr) {
            push_tail(otherCurrent->data);
            otherCurrent = otherCurrent->next;
        }
    }

    void pop_head() {
        if (head == nullptr) {
            throw runtime_error("List is empty");
        }

        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        else {
            tail = nullptr;
        }

        delete temp;
    }

    void pop_tail() {
        if (tail == nullptr) {
            throw runtime_error("List is empty");
        }

        Node* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }

        delete temp;
    }

    void delete_node(const T& value) {
        if (head == nullptr) {
            return;
        }

        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                if (current == head) {
                    head = current->next;
                    if (head != nullptr) {
                        head->prev = nullptr;
                    }
                    else {
                        tail = nullptr;
                    }
                }
                else if (current == tail) {
                    tail = current->prev;
                    if (tail != nullptr) {
                        tail->next = nullptr;
                    }
                    else {
                        head = nullptr;
                    }
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                Node* temp = current;
                current = current->next;
                delete temp;
            }
            else {
                current = current->next;
            }
        }
    }

    T& operator[](size_t index) const {
        if (index < 0) {
            throw out_of_range("Index is out of range");
        }

        Node* current = head;
        size_t currentIndex = 0;
        while (current != nullptr && currentIndex < index) {
            current = current->next;
            currentIndex++;
        }

        if (current == nullptr) {
            throw out_of_range("Index is out of range");
        }

        return current->data;
    }

    T& operator[](size_t index) {
        return const_cast<T&>(static_cast<const LinkedList&>(*this)[index]);
    }
};

template<typename T>
ostream& operator<<(ostream& os, const LinkedList<T>& list) {
    typename LinkedList<T>::Node* current = list.get_head();

    os << "[";

    while (current != nullptr) {
        os << current->data;
        current = current->next;

        if (current != nullptr) {
            os << ", ";
        }
    }

    os << "]";

    return os;
}

template <typename T>
LinkedList<T> sum_(const LinkedList<T>& list1, const LinkedList<T>& list2) {
    LinkedList<T> result;

    typename LinkedList<T>::Node* current1 = list1.get_tail();
    typename LinkedList<T>::Node* current2 = list2.get_tail();

    T carry = 0; // ѕеренос на следующий разр€д, изначально равен 0

    while (current1 != nullptr || current2 != nullptr) {
        T sum = carry;

        if (current1 != nullptr) {
            sum += current1->data;
            current1 = current1->prev;
        }

        if (current2 != nullptr) {
            sum += current2->data;
            current2 = current2->prev;
        }

        carry = sum / 10; // ¬ычисл€ем перенос на следующий разр€д
        T digit = sum % 10; // ¬ычисл€ем текущий разр€д результата

        result.push_head(digit);
    }

    if (carry != 0) {
        result.push_head(carry); // ≈сли осталс€ перенос, добавл€ем его в начало списка
    }

    return result;
}

template <typename T>
LinkedList<T> multiply_with_value(const LinkedList<T>& list, const T& value){ // умножение числа в виде списка на обычное число
    LinkedList<T> result;
    typename LinkedList<T>::Node* current = list.get_tail();
    T carry = 0;

    while (current != nullptr) {
        T product = current->data * value + carry;

        result.push_head(product % 10);
        carry = product / 10;

        current = current->prev;
    }

    // ≈сли есть перенос после последнего узла, добавл€ем новый узел
    if (carry > 0) {
        result.push_head(carry);
    }

    return result;
}

template <typename T>
LinkedList<T> multiply(const LinkedList<T>& num1, const LinkedList<T>& num2) {

    typename LinkedList<T>::Node* head1 = num1.get_head();
    typename LinkedList<T>::Node* head2 = num2.get_head();
    typename LinkedList<T>::Node* tail2 = num2.get_tail();

    if (head1 == nullptr || head2 == nullptr) {
        return LinkedList<T>();
    }

    LinkedList<T> result;
    typename LinkedList<T>::Node* current2 = tail2;
    int zero_count = 0;
    while (current2 != nullptr) {
        LinkedList<T> multy;
        multy = multiply_with_value(num1, current2->data);
        if (current2 != tail2) {
            for (int i = zero_count; i > 0; i--) { //если происходит умножение на разр€ды старше, чем разр€д единиц,
                multy.push_tail(0);                //в конце результирующего списка добавл€етс€ соотвественное количество нулей 
            }
        }
        result = sum_(result, multy);
        current2 = current2->prev;
        zero_count++;
    }
    return result;
}

int main() {
    LinkedList<int> list1;
    list1.push_head(3);
    list1.push_head(2);

    LinkedList<int> list2;
    list2.push_head(9);
    list2.push_head(8);
    list2.push_head(7);
    list2.push_head(6);

    LinkedList<int> sum = sum_(list1, list2);

    cout << sum << endl;

    LinkedList<int> list3;
    list3.push_tail(9);
    list3.push_tail(1);
    list3.push_tail(2);
    list3.push_tail(3);

    LinkedList<int> list4;
    list4.push_tail(4);
    list4.push_tail(5);
    list4.push_tail(6);

    // ”множение списков
    LinkedList<int> result = multiply(list3, list4);

    // ¬ывод результата
    cout << result;
    return 0;
}