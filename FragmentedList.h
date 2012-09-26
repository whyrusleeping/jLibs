#ifndef _FRAG_LIST_H_
#define _FRAG_LIST_H_

#include <iostream>
using std::cout;

template <class T>
struct _fragListNode
{
	struct _fragListNode *prev;
	struct _fragListNode *next;
	T data;
};

template <class T>
class FragmentedList
{
public:

	typedef struct _fragListNode<T> fragListNode;
	FragmentedList()
	{
		init();
	}

	FragmentedList(int initialSize)
	{
		init();
		resize(initialSize);	
	}

	~FragmentedList()
	{	
		clean();
	}

	void resize(int nSize)
	{
		// Should this actually resize the current block or
		// just reinitialize? Might call it reset.
		if ( blockSize != 0 || Size != 0 )
			clean();

		Size = nSize;
		blockSize = Size;
		freeSpots = NULL;
		list = new fragListNode[nSize];
		head = list;
		link();
	}

	void link()
	{
		//Link each node together, even though they are contiguous
		list[0].prev = NULL;
		for(int i = 0; i < Size-1; i++)
		{
			list[i].next = &list[i+1];
			list[i+1].prev = &list[i];
		}
		list[Size-1].next = NULL;
	}

	int size()
	{
		return Size;
	}

	void _defrag()
	{
		
	}

	void _swap_nodes(fragListNode * first, fragListNode * second)
	{
		fragListNode node_temp_first;
		fragListNode node_temp_second;

		fragListNode * first_addr 	= NULL;
		fragListNode * second_addr 	= NULL;

		// Copy over the contents
		node_temp_first 	= * first;
		node_temp_second 	= * second;

		// Store the addresses;
		first_addr 		= first;
		second_addr 	= second;

		// Swap the data
		(first)->data 	= node_temp_second.data
		(second)->data = node_temp_first.data;

		// Swap the points
		(second)->next = node_temp_first.prev;
		(first)->prev  = node_temp_second.next;

		// Swap the "surounding" pointers
		node_temp_first.prev->next = node_temp_second.next;
		node_temp_first.next->prev = node_temp_first.prev;


	}

	void defrag()
	{
		/*
		 * An attempt to just move the memory up 
		if(blockSize > Size)
		{
			T tempA;
			T tempB;
			fragListNode *it = head;
			int i = 0;
			
			for(; i < Size && it == &list[i]; i++, it = it->next);

			
		}*/
		if ( Size <= blockSize )
		{
			// Everyting will fit				
			fragListNode * list_temp = NULL, * head_temp = NULL;
			fragListNode node_temp;

			list_temp = list;
			head_temp = head;

			unsigned int sizeof_node = sizeof(fragListNode);
			
			// Move up through the block until the list gets out of sync
			for ( ; list_temp == head_temp; list_temp += sizeof_node, head_temp = head_temp->next );
						
				
			

		}
		else
		{
		  fragListNode *temp = new fragListNode[Size];
		  fragListNode *it = head;

		  for(int i = 0; i < Size; i++)
		  {
			  temp[i].data = it->data;
			  it = it->next;
		  }

		  head = temp;
		  delete[] list;
		  list = head;
		  link();	
		  blockSize = Size;		
		  fragmented = false;	
		}
	}

	T &operator[] (int index)
	{
		return at(index)->data;
	}

	fragListNode *at(int index)
	{
		if(!fragmented) return &list[index];
		else
		{
			if(index < lastIti || true)
			{
				lastIti = 0;
				lastIt = head;
			}			
			for(;lastIti < index; lastIti++, lastIt = lastIt->next);
			return lastIt;
		}
	}

	void push_front(T item)
	{
		fragListNode *nnode = new fragListNode;
		nnode->data = item;
		nnode->next = head;
		head->prev = nnode;
		head = nnode;
		nnode->prev = NULL;
		fragmented = true;
		Size++;
	}

	T pop_front()
	{
		T ret = head->data;
		erase(0);
		return ret;		
	}

	void push_back(T item)
	{
		//this function seems like it shouldnt need to fragment the list...
		fragListNode *nnode = new fragListNode;
		nnode->data = item;

		//just keep a tail pointer, idiot.
		nnode->prev = at(Size-1)->next;
		
		nnode->prev->next = nnode;
		nnode->next = NULL;
		Size++;

	}

	void insert(T item, int index)
	{	
		fragListNode *nnode = new fragListNode;
		/*
		//attempt to reuse memory
		if(freeSpots == NULL)
			nnode = new fragListNode;
		else
		{
			nnode = freeSpots;
			freeSpots = freeSpots->next;
		}*/

		nnode->data = item;
		fragListNode *temp = at(index);	
		nnode->next = temp;
		if(index > 0)
		{
			temp->prev->next = nnode;
			nnode->prev = temp->prev;
			temp->prev = nnode;
		}
		else
		{
			nnode->prev = NULL;
			temp->prev = nnode;
			head = nnode;
		}
		fragmented = true;
		Size++;
		if(index <= lastIti)
			lastIti++;
	}

	void erase(unsigned int index)
	{
		fragListNode *n = at(index);

		if(n->next == NULL)
		{
			n->prev->next = NULL;
		}
		else if(n->prev == NULL)
		{
			//keep separate memory pointer
			fragmented = true;
			n->next->prev = NULL;
			head = n->next;
		}
		else
		{
			fragmented = true;
			n->next->prev = n->prev;
			n->prev->next = n->next;
		}
		//manage the now unused memory`
		if(!(n >= list && n <= list + (sizeof(fragListNode) * blockSize)))
		{	
			delete n;
		}
		else
		{	
			n->next = freeSpots;
			freeSpots = n;
		}

		Size--;		
		if(index < lastIti)
		{
			lastIti--;
			return;
		}
		if(index == lastIti)
		{
			lastIti = 0;
			lastIt = head;
		}
	}
private:
	
	void clean()
	{
		fragListNode *temp = NULL;
		for(fragListNode *i = head; i != NULL;)
		{
			if(i >= list && i < list + (sizeof(fragListNode) * blockSize))
			{
				i = i->next;		
			}
			else
			{
				temp = i;
				i = i->next;
				delete temp;
			}
		}
		delete[] list;
		
		list = NULL;
		head = NULL;
		freeSpots = NULL;
		Size = 0;
		fragmented = false;
	}

	void init()
	{
	
		list = NULL;
		head = NULL;
		freeSpots = NULL;
		Size = 0;
		blockSize = 0;
		fragmented = false;
	}

	fragListNode *list; //a pointer to the lists main block of memory
	fragListNode *head;	//a pointer to the first item in the list
	fragListNode *lastIt; //a pointer to the last iterator used
	fragListNode *freeSpots; //a stack of unused memory spots in the main memory block;
	int lastIti; //index of the last iterator used
	int Size; //number of items in the list
	int blockSize; //size (in items) of the lists main memory block
	bool fragmented; 
};

#endif
