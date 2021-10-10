#pragma once
#include <deque>
#include <cassert>

/**
 * @brief Thread Safe Queue
 * @brief �̰߳�ȫ����
 */
template <typename T, typename D = deque<T> >
class TC_CasQueue
{
public:
    TC_CasQueue() :_size(0) {};

public:

    typedef D queue_type;

    /**
     * @brief Getting data from the head without data throwing exception
     * @brief ��ͷ����ȡ����, û�������׳��쳣
     *
     * @param t
     * @return bool: true, get data ; false, no data
     * @return bool: true, ��ȡ������, false, ������
     */
    T front();

    /**
     * @brief Get data from the head
     * @brief ��ͷ����ȡ����
     *
     * @param t
     * @return bool: true, get data ; false, no data
     * @return bool: true, ��ȡ������, false, ������
     */
    bool pop_front(T& t);

    /**
     * @brief Get data from the head
     * @brief ��ͷ����ȡ����
     *
     * @return bool: true, get data ; false, no data
     * @return bool: true, ��ȡ������, false, ������
     */
    bool pop_front();

    /**
     * @brief Put data to the back end of the queue.
     * @brief �����ݵ����к��.
     *
     * @param t
     */
    void push_back(const T& t);

    /**
     * @brief Put data to the back end of the queue.
     * @brief  �����ݵ����к��.
     *
     * @param vt
     */
    void push_back(const queue_type& qt);

    /**
     * @brief Put data to the front end of the queue.
     * @brief  �����ݵ�����ǰ��.
     *
     * @param t
     */
    void push_front(const T& t);

    /**
     * @brief Put data to the front end of the queue.
     * @brief  �����ݵ�����ǰ��.
     *
     * @param vt
     */
    void push_front(const queue_type& qt);

    /**
     * @brief  Exchange Data
     * @brief  ��������
     *
     * @param q
     * @return bool: true, data returned ; false, no data returned
     * @return �����ݷ���true, �����ݷ���false
     */
    bool swap(queue_type& q);

    /**
     * @brief  Size of the Queue
     * @brief  ���д�С.
     *
     * @return size_t the size of the queue
     * @return size_t ���д�С
     */
    size_t size() const;

    /**
     * @brief  Clear Up the Queue
     * @brief  ��ն���
     */
    void clear();

    /**
     * @brief  �Ƿ�����Ϊ��.
     * @brief  Determine whether the data is null or not.
     * @return bool Ϊ�շ���true�����򷵻�false
     * @return bool: true, is null ; false, not null
     */
    bool empty() const;

protected:
    TC_CasQueue(const TC_CasQueue&) = delete;
    TC_CasQueue(TC_CasQueue&&) = delete;
    TC_CasQueue& operator=(const TC_CasQueue&) = delete;
    TC_CasQueue& operator=(TC_CasQueue&&) = delete;

protected:
	/**
	 * ����
	 * Queue
	 */
	queue_type _queue;
	/**
	 * ���г���
	 * The length of a queue
	 */
	size_t _size;
	//��Lock
	std::mutex _mutex;
};

template <typename T, typename D> T TC_CasQueue<T, D>::front()
{
	std::lock_guard<std::mutex> lock(_mutex);

	return _queue.front();
}

template <typename T, typename D> bool TC_CasQueue<T, D>::pop_front(T& t)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_queue.empty())
	{
		return false;
	}

	t = _queue.front();
	_queue.pop_front();
	--_size;

	return true;
}

template <typename T, typename D> bool TC_CasQueue<T, D>::pop_front()
{
    std::lock_guard<std::mutex> lock(_mutex);
	if (_queue.empty())
	{
		return false;
	}

	_queue.pop_front();
    assert(_size > 0);
	--_size;

	return true;
}

template <typename T, typename D> void TC_CasQueue<T, D>::push_back(const T& t)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(t);
    ++_size;
}

template <typename T, typename D> void TC_CasQueue<T, D>::push_back(const queue_type& qt)
{
	std::lock_guard<std::mutex> lock(_mutex);

    typename queue_type::const_iterator it = qt.begin();
    typename queue_type::const_iterator itEnd = qt.end();
    while (it != itEnd) {
        _queue.push_back(*it);
        ++it;
        ++_size;
    }
}

template <typename T, typename D> void TC_CasQueue<T, D>::push_front(const T& t)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push_front(t);
	++_size;
}

template <typename T, typename D> void TC_CasQueue<T, D>::push_front(const queue_type& qt)
{
	std::lock_guard<std::mutex> lock(_mutex);

	typename queue_type::const_iterator it = qt.begin();
	typename queue_type::const_iterator itEnd = qt.end();
	while (it != itEnd) {
		_queue.push_front(*it);
		++it;
		++_size;
	}
}

template <typename T, typename D> bool TC_CasQueue<T, D>::swap(queue_type& qt)
{
	std::lock_guard<std::mutex> lock(_mutex);

    if (_queue.empty())
    {
        return false;
    }

    _queue.swap(qt);
    _size = _queue.size();
    return true;
}

template <typename T, typename D> size_t TC_CasQueue<T, D>::size()const
{
	std::lock_guard<std::mutex> lock(_mutex);
    return _size;
}

template <typename T, typename D> void TC_CasQueue<T, D>::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);
    _queue.clear();
    _size = 0;
}

template <typename T, typename D> bool TC_CasQueue<T, D>::empty() const
{
	std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}