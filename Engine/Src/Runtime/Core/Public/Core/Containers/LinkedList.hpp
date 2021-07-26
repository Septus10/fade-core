#pragma once

#include <type_traits>

namespace Fade {

	template <typename ObjectType>
	struct TListNode
	{
		TListNode(ObjectType& a_Object)
			: m_Next(nullptr)
			, m_Data(a_Object)
		{
		}

		TListNode(ObjectType&& a_Object)
			: m_Next(nullptr)
			, m_Data(std::move(a_Object))
		{
		}

		TListNode* m_Next;
		ObjectType m_Data;
	};

	template <typename ObjectType>
	class TLinkedListIterator;


	/**
	 * Singly linked list
	 */
	template <typename ObjectType>
	class TLinkedList
	{
		using pointer = ObjectType*;
		using AReference = ObjectType&;
		using ARvalReference = ObjectType&&;
		using AConstPtr = const ObjectType*;
		using AConstRef = const ObjectType&;

		TListNode<ObjectType>* m_First;
		TListNode<ObjectType>* m_Last;
		std::size_t m_Size;


	public:
		using AIterator = TLinkedListIterator<ObjectType>;

		/**
		 * Default constructor
		 */
		TLinkedList()
			: m_First(nullptr)
			, m_Last(nullptr)
			, m_Size(0)
		{
		}

		AIterator begin()
		{
			return AIterator(m_First);
		}

		AIterator begin() const
		{
			return AIterator(m_First);
		}

		AIterator end()
		{
			return AIterator(nullptr);
		}

		AIterator end() const
		{
			return AIterator(nullptr);
		}

		AReference First()
		{
			return *begin();
		}

		AReference Last()
		{
			return *end();
		}


		// If we add the first, both first and last are set to the node
		// Otherwise, only set the next of the current last to ours and set ours as the last

		void Add(AReference a_Object)
		{
			TListNode<ObjectType>* Node = new TListNode<ObjectType>(a_Object);
			// Check if we actually have a last
			if (m_Last != nullptr)
			{
				// Set our new node to be the next of our current last
				m_Last->m_Next = Node;
			}
			else // if we don't have a last
			{
				// Set our new now to be the first
				m_First = Node;
			}
			m_Last = Node;
			++m_Size;
		}

		void Add(ARvalReference a_Object)
		{
			TListNode<ObjectType>* Node = new TListNode<ObjectType>(std::move(a_Object));
			// Check if we actually have a last
			if (m_Last != nullptr)
			{
				// Set our new node to be the next of our current last
				m_Last->m_Next = Node;
			}
			else // if we don't have a last
			{
				// Set our new now to be the first
				m_First = Node;
			}
			m_Last = Node;
			++m_Size;
		}

		template <typename T>
		void Append(TLinkedList<T> a_Other)
		{
			if (m_Last) // If we have a last
			{
				// Append it to our last
				m_Last->m_Next = a_Other.m_First;
			}
			else // Otherwise
			{
				// Set it as our first
				m_First = a_Other.m_First;
				m_Last = a_Other.m_Last;
			}

			// Increment our size
			m_Size += a_Other.m_Size;
		}

		std::size_t Size() const
		{
			return m_Size;
		}

	private:
	};

	/**
	 * Iterator
	 */
	template <typename ObjectType>
	class TLinkedListIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = ObjectType;
		using difference_type = std::ptrdiff_t;
		using pointer = ObjectType*;
		using reference = ObjectType&;

		TLinkedListIterator() noexcept
			: m_Current(nullptr)
		{
		}

		TLinkedListIterator(TListNode<ObjectType>* a_Node) noexcept
			: m_Current(a_Node)
		{
		}

		TLinkedListIterator(const TListNode<ObjectType>* a_Node)
			: m_Current(const_cast<TListNode<ObjectType>*>(a_Node))
		{
		}

		TLinkedListIterator(std::nullptr_t) noexcept
			: m_Current(nullptr)
		{
		}

		reference operator*() const noexcept
		{
			return m_Current->m_Data;
		}

		pointer operator->() const noexcept
		{
			return m_Current->m_Data;
		}

		TLinkedListIterator& operator++() noexcept
		{
			m_Current = m_Current->m_Next;
			return *this;
		}

		TLinkedListIterator& operator++(int) noexcept
		{
			TLinkedListIterator Temp = *this;
			m_Current = m_Current->m_Next;
			return Temp;
		}

		TLinkedListIterator& operator+=(const difference_type a_Diff) noexcept
		{

		}

		bool operator!=(std::nullptr_t)
		{
			return m_Current != nullptr;
		}

		bool operator!=(const TListNode<ObjectType>* a_Ptr)
		{
			return m_Current != a_Ptr;
		}

		bool operator!=(TListNode<ObjectType>* a_Ptr)
		{
			return m_Current != a_Ptr;
		}

		template <typename T>
		bool operator!=(const TLinkedListIterator<T>& a_Other)
		{
			return m_Current != a_Other.m_Current;
		}

	private:
		TListNode<ObjectType>* m_Current;
	};


}