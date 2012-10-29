// vim: noai ts=4 sw=4 et!:
#ifndef _FRAG_LIST_H_
#define _FRAG_LIST_H_

#include <iostream>
#include <iomanip>
#include <assert.h>

#define DEBUG_ENABLED false 
#define DEBUG_CODE()  if ( DEBUG_ENABLED )

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
class FragIterator
{
	typedef struct _fragListNode<T> fragListNode;
private:
	fragListNode *node;

public:
	FragIterator();
	FragIterator(fragListNode*);


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
		open_memory_ = NULL;
	}

	~FragmentedList()
	{
		clean();
	}

	void resize(const int nSize)
	{
		assert(nSize >= 0);
		if (fragmented_)
        {
            if ( memory_block_size_ != 0 || current_size_ != 0 )
    			clean();
    		memory_block_head_	= new fragListNode[nSize];
            //Data still lost.
        }
        else
        {
            if(memory_block_head_ != NULL)
                memory_block_head_ = new(memory_block_head_) fragListNode[nSize];
            else
                memory_block_head_ = new fragListNode[nSize];
        }
        current_size_		= nSize;
		memory_block_size_	= current_size_;
		open_memory_		= NULL;
		list_head_			= memory_block_head_;
		last_access_index_	= 0;
		list_last_position_ = list_head_;

		link();
	}

	void link()
	{
		//Link each node together, even though they are contiguous
		memory_block_head_[0].prev = NULL;
		for(int i = 0; i < current_size_-1; i++)
		{
			memory_block_head_[i].next		= &memory_block_head_[i+1];
			memory_block_head_[i+1].prev	= &memory_block_head_[i];
		}

		memory_block_head_[current_size_-1].next = NULL;
	}

	int size()
	{
		return current_size_;
	}


	void defrag()
	{
		DEBUG_CODE()
		{
			cout << "\n\n=================== DEFRAG ==================\n";
			cout <<     "=================== DEFRAG ==================\n";

			cout <<     "  Block size [" << current_size_ << " <= " << memory_block_size_ << "]\n";
			cout <<     "=============================================\n";
			cout <<     "=============================================\n";
		}

		if ( current_size_ <= memory_block_size_ )
		{
			// Everyting will fit
			fragListNode * block_head_temp	= NULL, * list_head_temp = NULL;
			fragListNode * node_temp		= NULL;
			fragListNode * free_temp		= NULL;
			int block_head_offset			= 0;
			int defrag_count				= 0;

			block_head_temp = memory_block_head_;
			list_head_temp	= list_head_;
			

			while( block_head_temp->next != NULL && block_head_temp->freeFlag == 0 )
			{
				
				DEBUG_CODE()
				{
					free_temp = open_memory_;

					block_head_offset = 0;

					DEBUG_CODE()
					{	
						cout << "\n\n\nBefore Shift | "<< FreeFlag(block_head_temp)<< " - block_head_temp[" << block_head_temp << "] "<< FreeFlag(list_head_temp)<< " - list_head_temp[" << list_head_temp << "] "<< FreeFlag(node_temp)<< " - node_temp["<< node_temp <<"]\n";
						cout << "\n\n================ DEFRAG [" << std::setfill('0') << std::setw(3) <<defrag_count << "] =============== ";
						cout << "   open_memory_ \n\n";
					}

					defrag_count++;

					while ( block_head_offset < memory_block_size_ )
					{
						DEBUG_CODE()
						{	
							cout << FreeFlag(memory_block_head_ + block_head_offset) << " :: ";
							cout << "[" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->prev << "] <=" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset) << "{" << (int)(memory_block_head_ + block_head_offset)->data << "}" << "=> [" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->next << "]";
						}

						if ( free_temp != NULL)
						{
							DEBUG_CODE()	
							{
								cout << "    [" << std::setfill('0')<< std::setw(9) << free_temp << "]";
							}

							free_temp = free_temp->next;
						}

						DEBUG_CODE()
						{	
							cout << "\n";
						}

						block_head_offset++;
					}
				}

				// Move up through the block until the memory_block_head_ gets out of sync
				for ( ; block_head_temp == list_head_temp && block_head_temp->next != NULL ; block_head_temp += 1, list_head_temp = list_head_temp->next );


				// No free spots?
				unsigned short allocated_mem = 0;
				unsigned short push_mem_to_stack = 0;
				node_temp = NULL;
				if ( open_memory_ != NULL )
				{
					push_mem_to_stack = 1;
					if ( block_head_temp != open_memory_ && block_head_temp->freeFlag == 1 )
					{
						// Move memory to front of open_memory_
						//_swap_nodes(block_head_temp, open_memory_);
						shift_to_front_of_free_memory(block_head_temp);
					}
					node_temp = get_free_spot();
				}

				DEBUG_CODE()
				{
					cout << "After Shift | "<< FreeFlag(block_head_temp)<< " - block_head_temp[" << block_head_temp << "] "<< FreeFlag(list_head_temp)<< " - list_head_temp[" << list_head_temp << "] "<< FreeFlag(node_temp)<< " - node_temp["<< node_temp<<"]\n";
				}

				unsigned char swapped_temp = 0 ;



				if ( block_head_temp == node_temp && block_head_temp->freeFlag == 1 )
				{
					//cout << "List = Temp\n";
					_swap_nodes(block_head_temp, list_head_temp);
					move_to_free(list_head_temp);
				}
				else if ( block_head_temp != node_temp && block_head_temp->freeFlag == 1 )
				{
					_swap_nodes(block_head_temp, list_head_temp);
					move_to_free(list_head_temp);
				}
				else if ( block_head_temp->freeFlag == 0 && list_head_temp->freeFlag == 0 )
				{
					_swap_nodes(block_head_temp, list_head_temp);
					if ( node_temp != NULL )
						move_to_free(node_temp);

				}

				list_head_temp = block_head_temp;
			}
			free_temp = open_memory_;
			block_head_offset = 0;

			DEBUG_CODE()
			{
				cout << "\n\n============ ENDING FRAGMENTATION =========== ";
				cout << "   open_memory_ \n\n";



				while ( block_head_offset < memory_block_size_ )
				{
					cout << FreeFlag(memory_block_head_ + block_head_offset) << " :: ";
					cout << "[" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->prev << "] <=" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset) << "{" << (int)(memory_block_head_ + block_head_offset)->data << "}" << "=> [" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->next << "]";

					if ( free_temp != NULL)
					{
						cout << "    [" << std::setfill('0')<< std::setw(9) << free_temp << "]";

						free_temp = free_temp->next;
					}

					cout << "\n";
					block_head_offset++;
				}
			}
		}
		else
		{
			fragListNode *temp = new fragListNode[current_size_];
			fragListNode *it = list_head_;

			for(int i = 0; i < current_size_; i++)
			{
				temp[i].data = it->data;
				it = it->next;
				temp[i].freeFlag = 0;
			}

			list_head_ = temp;

			//open_memory_ was keeping track of memory that was being deleted, and when i went to delete it, was causing the system to freak out
			open_memory_ = NULL;
			delete[] memory_block_head_;
			memory_block_head_ = list_head_;
			link();
			memory_block_size_ = current_size_;
			fragmented_ = false;
			last_access_index_ = 0;
			list_last_position_ = list_head_;
		}
	}

	T &operator[] (int index)
	{
		return at(index)->data;
	}

	fragListNode *at(int index)
	{
		assert(memory_block_head_ != NULL);
		assert(index >= 0);
		if(!fragmented_) return &memory_block_head_[index];
		else
		{
			if(index < last_access_index_)// || true)
			{
				last_access_index_ = 0;
				list_last_position_ = list_head_;
			}
			for(; last_access_index_ < index; last_access_index_++, list_last_position_ = list_last_position_->next);
			return list_last_position_;
		}
	}

	void push_front(T item)
	{
		assert(list_head_ != NULL);
		fragListNode *nnode = new fragListNode;
		nnode->data = item;
		nnode->next = list_head_;
		list_head_->prev = nnode;
		list_head_ = nnode;
		nnode->freeFlag = 0;
		nnode->prev = NULL;
		fragmented_ = true;
		current_size_++;
	}

	T pop_front()
	{
		assert(list_head_ != NULL);
		T ret = list_head_->data;
		erase(0);
		return ret;
	}

	void push_back(T item)
	{
		//this function seems like it shouldnt need to fragment the memory_block_head_...
		fragListNode *nnode = new fragListNode;
		nnode->data = item;

		//just keep a tail pointer, idiot.
		nnode->prev = at(current_size_-1)->next;

		nnode->freeFlag		= 0;
		nnode->prev->next	= nnode;
		nnode->next			= NULL;
		current_size_++;

	}

	void insert(T item, int index)
	{
		assert(index >= 0);
		fragListNode *nnode;// = new fragListNode;

		//attempt to reuse memory
		if(open_memory_ == NULL)
			nnode = new fragListNode;
		else
		{
			nnode			= open_memory_;
			open_memory_	= open_memory_->next;

			if ( open_memory_ != NULL )
				open_memory_->prev = NULL;
		}

		nnode->prev			= NULL;
		nnode->next			= NULL;
		nnode->freeFlag		= 0;

		nnode->data			= item;
		fragListNode *temp	= at(index);
		nnode->next			= temp;

		if(index > 0)
		{
			temp->prev->next	= nnode;
			nnode->prev			= temp->prev;
			temp->prev			= nnode;
		}
		else
		{
			nnode->prev = NULL;
			temp->prev	= nnode;
			list_head_	= nnode;
		}

		fragmented_ = true;
		current_size_++;
		if(index <= last_access_index_)
			last_access_index_++;
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
			fragmented_		= true;
			n->next->prev	= NULL;
			list_head_		= n->next;
		}
		else
		{
			fragmented_		= true;
			n->next->prev	= n->prev;
			n->prev->next	= n->next;
		}
		//manage the now unused memory`
		if(!(n >= memory_block_head_ && n <= memory_block_head_ + (sizeof(fragListNode) * memory_block_size_)))
		{
			delete n;
		}
		else
		{
			move_to_free(n);
		}


		current_size_--;
		if(index < last_access_index_)
		{
			last_access_index_--;
			return;
		}
		if(index == last_access_index_)
		{
			last_access_index_	= 0;
			list_last_position_ = list_head_;
		}
	}
	void dumpDebug()
	{
		fragListNode * free_temp		= NULL;
		fragListNode * block_head_temp	= NULL;

		block_head_temp	= memory_block_head_;
		free_temp		= open_memory_;

		int block_head_offset = 0;

		cout << "\n\n================ DEFRAG [" << std::setfill('0') << "] =============== ";
		cout << "   open_memory_ \n\n";

		block_head_offset = 0;
		while ( block_head_offset < memory_block_size_ )
		{
			if ( (memory_block_head_ + block_head_offset)->freeFlag )
			{	
				cout << "F :: ";
			}
			else
			{	
				cout << "L :: ";
			}
			
			cout << "[" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->prev << "] <=" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset) << "{" << (int)(memory_block_head_ + block_head_offset)->data << "}" << "=> [" << std::setfill('0')<< std::setw(9) << (memory_block_head_ + block_head_offset)->next << "]";

			if ( free_temp != NULL)
			{
				cout << "    [" << std::setfill('0')<< std::setw(9) << free_temp << "]";
				free_temp = free_temp->next;
			}

			cout << "\n";
			block_head_offset++;
		}
	}

private:
	char FreeFlag(fragListNode * node)
	{
		if ( node == NULL ) return 'N';
		return (node->freeFlag == 1) ? 'F' : 'L';
	}

	void move_to_free(fragListNode * node)
	{
		node->next = open_memory_;
		if (open_memory_ != NULL )
			open_memory_->prev = node;
		node->prev = NULL;
		node->freeFlag = 1;
		open_memory_ = node;

	}
	fragListNode * get_free_spot ()
	{
		fragListNode* node = NULL;
		node = open_memory_;
		if ( node != NULL )
		{
			open_memory_ = open_memory_->next;

			if ( open_memory_ != NULL )
				open_memory_->prev = NULL;
			// node->prev = NULL;
			// node->next = NULL;
		}
		return node;

	}



	void shift_to_front_of_free_memory(fragListNode * node)
	{
		fragListNode temp;

		if (node->prev == NULL)
			return;

		temp.data     = node->data;
		temp.freeFlag = node->freeFlag;
		temp.next     = node->next;
		temp.prev     = node->prev;


		// Puts node at front
		open_memory_->prev = node;
		node->next = open_memory_;
		node->prev = NULL;
		open_memory_ = node;


		if(temp.next == NULL)
		{
			temp.prev->next = NULL;
		}
		else
		{
			temp.next->prev = temp.prev;
			temp.prev->next = temp.next;
		}

	}
	void _swap_nodes(fragListNode * first, fragListNode * second)
	{
		assert(first != NULL);
		assert(second != NULL);

		fragListNode node_temp_first;
		fragListNode node_temp_second;

		fragListNode * first_addr	= NULL;
		fragListNode * second_addr	= NULL;


		// Copy over the contents
		node_temp_first  = *first;
		node_temp_second = *second;


		// Store the addresses;
		first_addr	= first;
		second_addr	= second;


		// Swap the first
		(first)->freeFlag	= node_temp_second.freeFlag;
		(first)->data		= node_temp_second.data;
		(first)->prev		= node_temp_second.prev;


		if ( node_temp_second.next == first_addr )
			(first)->next  = node_temp_first.prev;
		else
			(first)->next  = node_temp_second.next;


		// Swap the second
		(second)->freeFlag	= node_temp_first.freeFlag;
		(second)->data		= node_temp_first.data;
		(second)->next		= node_temp_first.next;


		if ( node_temp_first.prev == second_addr )
			(second)->prev = node_temp_second.next;
		else
			(second)->prev = node_temp_first.prev;


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

	void clean()
	{
		fragListNode *temp = NULL;
		for(fragListNode *i = list_head_; i != NULL;)
		{
			if(i >= memory_block_head_ && i < memory_block_head_ + (sizeof(fragListNode) * memory_block_size_))
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
		delete[] memory_block_head_;

		memory_block_head_	= NULL;
		list_head_			= NULL;
		open_memory_		= NULL;
		current_size_		= 0;
		memory_block_size_	= 0;
		fragmented_			= false;
	}

	void init()
	{

		memory_block_head_	= NULL;
		list_head_			= NULL;
		open_memory_		= NULL;
		current_size_		= 0;
		memory_block_size_	= 0;
		fragmented_			= false;
	}

	fragListNode *memory_block_head_; //a pointer to the lists main block of memory
	fragListNode *list_head_;	//a pointer to the first item in the memory_block_head_
	fragListNode *list_last_position_; //a pointer to the last iterator used
	fragListNode *open_memory_; //a stack of unused memory spots in the main memory block;
	int last_access_index_; //index of the last iterator used
	int current_size_; //number of items in the memory_block_head_
	int memory_block_size_; //size (in items) of the lists main memory block
	bool fragmented_;
};

#endif
