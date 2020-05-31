#if !defined(LINKED_LIST)
#define LINKED_LIST

template <typename T>
struct Node
{
    T data;
    Node<T> *prev;
    Node<T> *next;

    Node(T data, Node *prev, Node *next)
    {
        this->data = data;
        this->prev = prev;
        this->next = next;
    }
};

template <typename T>
class LinkedList
{
private:
public:
    Node<T> *mHead;
    LinkedList()
    {
        mHead = nullptr;
    }

    Node<T> *pushBack(T tValue)
    {
        Node<T> *newNode = new Node<T>(tValue, nullptr, nullptr);
        Node<T> *tail = mHead;

        if (mHead == nullptr)
        {
            mHead = newNode;
            return newNode;
        }

        while (tail->next != nullptr)
        {
            tail = tail->next;
        }

        tail->next = newNode;
        newNode->prev = tail;
        return newNode;
    }

    Node<T> *pushBackOf(Node<T> *tPrev, T tValue)
    {
        Node<T> *newNode = new Node<T>(tValue, nullptr, nullptr);

        if (tPrev == nullptr)
        {
            return nullptr;
        }

        newNode->next = tPrev->next;
        tPrev->next->prev = newNode;

        tPrev->next = newNode;
        newNode->prev = tPrev;
        return newNode;
    }

    ~LinkedList()
    {
    }
};

#endif
