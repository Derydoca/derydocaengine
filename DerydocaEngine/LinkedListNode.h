#pragma once
template <class T>
class LinkedListNode
{
public:
	LinkedListNode() {}
	~LinkedListNode() {}

	inline T* left() const { return m_left; }
	inline T* right() const { return m_right; }

	void removeSelf()
	{
		if (m_left == m_right) {
			return;
		}
		m_left->setRight(m_right);
		m_right->setLeft(m_left);
	}
	void addLeft(T* other)
	{
		m_left->setRight(other);
		setLeft(other);
	}
	void addRight(T* other)
	{
		m_right->setLeft(other);
		setRight(other);
	}
protected:
	T* setLeft(T* other) { m_left = other; }
	T* setRight(T* other) { m_right = other; }
private:
	T* m_left;
	T* m_right;
};

