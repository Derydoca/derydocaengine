#pragma once
template <class T>
class LinkedListNode
{
public:
	LinkedListNode() {}
	LinkedListNode(T* data) :
		m_data(data)
	{

	}
	~LinkedListNode() {}

	inline LinkedListNode* prev() const { return m_prev; }
	inline LinkedListNode* next() const { return m_next; }
	inline T* data() { return m_data; }

	void removeSelf()
	{
		if (m_prev != NULL) {
			if (m_next == NULL) {
				m_next->setPrev(NULL);
			}
			else {
				m_next->setPrev(m_prev);
				m_prev->setNext(m_next);
			}
		}
		else if (m_next != null) {
			m_next->setPrev(NULL);
		}

		setPrev(NULL);
		setNext(NULL);
	}
	void addPrev(LinkedListNode* const& other)
	{
		if (m_prev == NULL) {
			other->setNext(this);
		}
		else {
			m_prev->setNext(other);
			other->setPrev(m_prev);
			other->setNext(this);
		}
		setPrev(other);
	}
	void addNext(LinkedListNode* const& other)
	{
		if (m_next == NULL) {
			other->setPrev(this);
		}
		else {
			m_next->setPrev(other);
			other->setNext(m_next);
			other->setPrev(this);
		}
		setNext(other);
	}
protected:
	void setPrev(LinkedListNode* const& other) { m_prev = other; }
	void setNext(LinkedListNode* const& other) { m_next = other; }
private:
	T* m_data;
	LinkedListNode* m_prev;
	LinkedListNode* m_next;
};
