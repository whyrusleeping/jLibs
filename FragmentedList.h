#ifndef _FRAG_LIST_H_
#define _FRAG_LIST_H_

#include <iostream>
#include <iomanip>
using std::cout;

template <class T>
struct _fragListNode
{
	struct _fragListNode *prev;
	struct _fragListNode *next;
	T data;
	unsigned char freeFlag;
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
		freeSpots = NULL;
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
		lastIti = 0;
		lastIt = head;
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

	void _swap_nodes(fragListNode * first, fragListNode * second)
	{
		//cout << "_swap_nodes\n";
	//	cout << "first[" << std::hex << first << "] second[" <<  std::hex << second << "]\n";

		fragListNode node_temp_first;
		fragListNode node_temp_second;

		fragListNode * first_addr 	= NULL;
		fragListNode * second_addr 	= NULL;

		// Copy over the contents
		node_temp_first.data      = first->data;
		node_temp_first.freeFlag  = first->freeFlag;
		node_temp_first.next  = first->next;
		node_temp_first.prev  = first->prev;


		node_temp_second.data 	  = second->data;
		node_temp_second.freeFlag = second->freeFlag;
		node_temp_second.next = second->next;
		node_temp_second.prev = second->prev;

		// Store the addresses;
		first_addr 	= first;
		second_addr	= second;

		// Swap the first
		(first)->freeFlag  = node_temp_second.freeFlag;
		(first)->data  = node_temp_second.data;
		(first)->prev  = node_temp_second.prev;
		
		if ( node_temp_second.next == first_addr )  
			(first)->next  = node_temp_first.prev;
		else
			(first)->next  = node_temp_second.next;


		// Swap the second
		(second)->freeFlag = node_temp_first.freeFlag;
		(second)->data = node_temp_first.data;
		(second)->next = node_temp_first.next;

		if ( node_temp_first.prev == second_addr )  
		  (second)->prev = node_temp_second.next;
		else
		  (second)->prev = node_temp_first.prev;



		//cout << "first.data ["<<first->data<<"] second.data ["<<second->data<<"]\n";
		


		// Swap the "surounding" pointers
		if (node_temp_second.prev != NULL)
			node_temp_second.prev->next = first_addr;

		if (node_temp_first.next != NULL)
			node_temp_first.next->prev = second_addr;

		if (node_temp_first.prev != NULL && node_temp_first.prev != second_addr  )
			node_temp_first.prev->next = second_addr;

		if (node_temp_second.next != NULL && node_temp_second.next != first_addr )
			node_temp_second.next->prev = first_addr;

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
		cout << "\n\n=================== DEFRAG ==================\n";
		cout <<     "=================== DEFRAG ==================\n";

		cout << "  Block size [" << Size << " <= " << blockSize << "]\n";
		cout <<     "=============================================\n";
		cout <<     "=============================================\n";
		if ( Size <= blockSize )
		{
		  // Everyting will fit				
			fragListNode * list_temp = NULL, * head_temp = NULL;
			fragListNode * node_temp = NULL;
			fragListNode * free_temp = NULL;
			int shift = 0;
			int defrag_count = 0;
			unsigned int sizeof_node = sizeof(fragListNode);
		
			list_temp = list;
			head_temp = head;
			while( list_temp->next != NULL )
			{
			  free_temp = freeSpots;
			  cout << "\n\n================ DEFRAG [" << std::setfill('0') << std::setw(3) <<defrag_count << "] =============== ";
			  cout << "   freeSpots \n\n"; 
			  defrag_count++;
			  shift = 0;
			  while ( shift < blockSize )
			  { 
			  	if ( (list + shift)->freeFlag )
			  	  cout << "F :: ";
				else
				  cout << "L :: ";
				  cout << "[" << std::setfill('0')<< std::setw(9) << (list + shift)->prev << "] <=" << std::setfill('0')<< std::setw(9) << (list + shift) << "{" << (int)(list + shift)->data << "}" << "=> [" << std::setfill('0')<< std::setw(9) << (list + shift)->next << "]";
				  
				if ( free_temp->next != NULL )
			  	{
					cout << "    [" << std::setfill('0')<< std::setw(9) << free_temp << "]";  
					free_temp = free_temp->next;
				}
				
				cout << "\n";
				shift++;
			  }
			  
			  // Move up through the block until the list gets out of sync
			  for ( ; list_temp == head_temp && list_temp->next != NULL ; list_temp += 1, head_temp = head_temp->next )
			  {
			///	  cout << "1list_temp[" << list_temp << "] head_temp[" << head_temp << "]\n";
			  }
			//	  cout << "2list_temp[" << list_temp << "] head_temp[" << head_temp << "]\n";
		 
		 	  if ( list_temp->next == NULL )
			  break;
			  // No free spots?
			  unsigned short allocated_mem = 0;
			  unsigned short push_mem_to_stack = 0;
			  if ( freeSpots != NULL )
			  {
				  push_mem_to_stack = 1;
				  node_temp = freeSpots;
				  node_temp->freeFlag = 0;
				  freeSpots = freeSpots->next;
			  }
			  else if( list_temp == freeSpots )
			  {
				  allocated_mem = 1;
				  node_temp = new fragListNode;
			          node_temp->freeFlag = 0;
				  node_temp->next = NULL;
				  node_temp->prev = NULL;
			  }

			// TODO HERE START
			  //cout << "3list_temp[" << list_temp << "] head_temp[" << head_temp << "]\n";
			  if ( (list_temp != node_temp && list_temp->freeFlag == 1 && node_temp->freeFlag == 1)
			  	|| (list_temp != node_temp && list_temp->freeFlag == 0 && node_temp->freeFlag == 1)  )
			  {
				 
				  _swap_nodes(list_temp, node_temp);
			//	  cout << "4list_temp[" << list_temp << "] node_temp[" << node_temp << "]\n";
			  }
		  
			  if ( head_temp != NULL )
				  _swap_nodes(list_temp, head_temp);
		  
		          list_temp->freeFlag = 0;
			  //cout << "5list_temp[" << list_temp << "] head_temp[" << head_temp << "]\n";
		  
			  if ( allocated_mem == 0 && push_mem_to_stack == 1 )
			  {
				 // cout << "Pushing back to stack\n";
				  head_temp->next = freeSpots;
				  head_temp->prev = NULL;
				  head_temp->freeFlag = 1;
				  freeSpots = head_temp;
			  }

			  if ( allocated_mem == 1 )
			  	delete node_temp;


			  head_temp = list_temp;
			//  std::cin >> a;
			}
		}
		else
		{ 
		  fragListNode *temp = new fragListNode[Size];
		  fragListNode *it = head;

		  for(int i = 0; i < Size; i++)
		  {
			  temp[i].data = it->data;
			  it = it->next;
			  temp[i].freeFlag = 0;
		  }

		  head = temp;
		  
		  //freeSpots was keeping track of memory that was being deleted, and when i went to delete it, was causing the system to freak out
		  freeSpots = NULL;
		  delete[] list;
		  list = head;
		  link();	
		  blockSize = Size;		
		  fragmented = false;	
		  lastIti = 0;
		  lastIt = head;
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
			if(index < lastIti)// || true)
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
		nnode->freeFlag = 0;
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
		
		nnode->freeFlag = 0;
		nnode->prev->next = nnode;
		nnode->next = NULL;
		Size++;

	}

	void insert(T item, int index)
	{	
		fragListNode *nnode;// = new fragListNode;
	
		//attempt to reuse memory
		if(freeSpots == NULL)
			nnode = new fragListNode;
		else
		{
			nnode = freeSpots;
			freeSpots = freeSpots->next;
		}

		nnode->freeFlag = 0;

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
			n->prev = NULL;
			n->freeFlag = 1;
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
