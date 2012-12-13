#ifndef J_ASYNC_QUEUE_H
#define J_ASYNC_QUEUE_H
#include <mutex>
using std::mutex;

//Thread Safe Queue
//  I had a need for one, so I wrote one.
//  add -std=gnu++11 -pthread to your compiler flags (depending on compiler of course)

template <class T>
class queueNode
{
	public:
		queueNode(T data){_data = data; next = NULL;}
		T _data;
		queueNode<T> *next;
};

template <class T>
class asyncQueue
{
	public:
		asyncQueue();

		void enqueue(T data);
		T dequeue();
		bool empty();
	private:
		queueNode<T> *top;
		queueNode<T> *bot;
		mutex qLock;
		
};


template <class T>
asyncQueue<T>::asyncQueue()
{
	top = NULL;
	bot = NULL;
}

template <class T>
void asyncQueue<T>::enqueue(T data)
{
	queueNode *toIns = new queueNode(data);
	qLock.lock();
	if(bot != NULL)
	{
		bot->next = toIns;
		bot = toIns;
	}
	else
	{
		top = toIns;
		bot = toIns;
	}
	qLock.unlock();
}

template <class T>
T asyncQueue<T>::dequeue()
{
	T ret;
	queueNode<T> *cur = NULL;
	qLock.lock();
	cur = top;
	top = top->next;
	if(!top)
		bot = NULL;
	qLock.unlock();
	ret = cur->_data;
	delete cur;
	return ret;
}

template <class T>
bool asyncQueue<T>::empty()
{
	bool ret;
	qLock.lock();
	ret = (top == NULL);
	qLock.unlock();
	return ret;
}
#endif 
