#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename Type>
class SingleLinkedList
{
    struct Node
    {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), next_node(next) {}

        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator
    {
    private:

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {}

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;


        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept
        {
            assert(node_ != nullptr);

            node_ = node_->next_node;
            return *this;
        }

        BasicIterator& operator++(int) noexcept
        {
            assert(node_ != nullptr);

            auto old_value(*this);
            ++(*this);

            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept
        {
            assert(node_ != nullptr);

            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept
        {
            assert(node_ != nullptr);

            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{ nullptr };
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{ nullptr };
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return begin();
    }

    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return end();
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{ &head_ };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return cbefore_begin();
    }

    SingleLinkedList() :
        head_(), size_(0) {}

    SingleLinkedList(initializer_list<Type> values)
    {
        assign(values);
    }

    SingleLinkedList(const SingleLinkedList& other)
    {
        assert(size_ == 0 && head_.next_node == nullptr);

        assign(other);
    }

    ~SingleLinkedList()
    {
        clear();
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        if (this != &rhs)
        {
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }

    Iterator insert(ConstIterator pos, const Type& value)
    {
        assert(pos.node_ != nullptr);

        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator{ pos.node_->next_node };
    }

    Iterator erase(ConstIterator pos) noexcept
    {
        assert(!IsEmpty());
        assert(pos.node_ != nullptr);

        auto temp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = temp;
        --size_;

        return Iterator{ pos.node_->next_node };
    }

    void swap(SingleLinkedList& other) noexcept
    {
        Node* tempHead = head_.next_node;
        size_t tempSize = size_;

        head_.next_node = other.head_.next_node;
        size_ = other.size_;

        other.head_.next_node = tempHead;
        other.size_ = tempSize;
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        if (size_ == 0)
        {
            return true;
        }
        return false;
    }

    void push_front(const Type& value)
    {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    void clear() noexcept
    {
        while (head_.next_node)
        {
            Node* new_node_ = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_node_;
        }
        size_ = 0;
    }

    void pop_front() noexcept
    {
        assert(!IsEmpty());

        auto new_head = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = new_head;
        --size_;
    }

private:
    Node head_;
    size_t size_;

    template<typename T>
    void assign(T& elem)
    {
        SingleLinkedList tmp;
        SingleLinkedList tmp_reverse;

        for (auto it = elem.begin(); it != elem.end(); ++it)
        {
            tmp_reverse.PushFront(*it);
        }
        for (auto it = tmp_reverse.begin(); it != tmp_reverse.end(); ++it)
        {
            tmp.PushFront(*it);
        }

        swap(tmp);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !(lhs < rhs);
}

int main()
{

}