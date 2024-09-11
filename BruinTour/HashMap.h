
#ifndef HASHMAP_INCLUDED
#define HASHMAP_INCLUDED

#include <list>
#include <vector>
#include <string>
using namespace std;

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75) // constructor
	{
		m_maxLoad = max_load;
		m_numBuckets = 10;
		m_hashmap.resize(m_numBuckets); //Empty hashmaps start with 10 buckets
		m_count = 0;
	}
	~HashMap() {} // destructor; deletes all of the items in the hashmap
	int size() const { return m_count; } // return the number of associations in the hashmap
	// The insert method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the hashmap with that key/value pair. If there is
	// already an association with that key in the hashmap, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the hashmap must contain no duplicate keys.
	void insert(const std::string& key, const T& value)
	{
		double load_factor = (m_count + 1) / m_numBuckets;
		if (load_factor > m_maxLoad)
			reallocate();
		int h = hash(key, m_numBuckets);
		if (find(key) == nullptr)
		{
			m_hashmap[h].push_back(Node(key, value));
			m_count++;
		}
		else
		{
			for (Node& n : m_hashmap[h])
				if (n.key == key)
				{
					n.value = value;
					return;
				}
		}
	}
	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.
	const T* find(const std::string& key) const
	{
		int h = hash(key, m_numBuckets);
		for (const Node& n : m_hashmap[h])
			if (n.key == key)
				return (&n.value);
		return nullptr;
	}
	// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T. Then it will return a
	// reference to the newly created value in the map.
	T* find(const std::string& key) {
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}

	T& operator[](const std::string& key)
	{
		int h = hash(key, m_numBuckets);
		if (find(key) == nullptr)
		{
			double load_factor = (m_count + 1) / m_numBuckets;
			if (load_factor > m_maxLoad)
			{
				reallocate();
				h = hash(key, m_numBuckets);
			}
			m_hashmap[h].push_back(Node(key, T()));
			m_count++;
		}
		for (Node& n : m_hashmap[h])
			if (n.key == key)
				return n.value;
		//Because of error "not all control paths return a value"
		throw std::out_of_range("Key not found");
	}

private:
	struct Node {
		string key;
		T value;

		Node(const string& k, const T& v) : key(k), value(v) {}
	};
	vector<list<Node>> m_hashmap;
	double m_maxLoad;
	int m_numBuckets;
	double m_count; //Num of associations in map

	//HashMap(const HashMap&) = delete;
	//HashMap& operator=(const HashMap&) = delete;

	int hash(string key, int nBuckets) const {
		size_t h = std::hash<std::string>()(key);
		return h % nBuckets;
	}
	void reallocate()
	{
		HashMap<T> newMap(m_maxLoad); 
		//Copy variables over
		newMap.m_numBuckets = m_numBuckets * 2;
		newMap.m_hashmap.resize(newMap.m_numBuckets);
		newMap.m_count = m_count;
		//Loop through all buckets and rehash items according to new bucket size
		for (int i = 0; i < m_hashmap.size(); i++)
		{
			for (Node& n : m_hashmap[i])
			{
				int h = hash(n.key, newMap.m_numBuckets);
				newMap.m_hashmap[h].push_back(n);
			}
		}

		std::swap(*this, newMap);
	}
};

#endif