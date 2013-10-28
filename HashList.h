#include <vector>
#include <unordered_map>

template <class Ty_>
class HashList
{
private:
	typedef unsigned long long Code_;
	Code_ ptrToCode_(void * x, void * y) const;
	void * codeToDst_(Code_ code) const;
	void * codeToSrc_(Code_ code) const;

	std::unordered_map<Code_, Code_> nextIn_;
	std::unordered_map<Code_, Code_> nextOut_;
	std::unordered_map<void *, Code_> headsIn_;
	std::unordered_map<void *, Code_> headsOut_;
public:
	HashList(){};
	~HashList(){};

	void insertEdge(void * x, void * y);
	void insertVertex(void * v);

	std::vector<void*> getAdjacent(void * v) const;
	std::vector<void*> getIn(void * v) const;
	std::vector<void*> getOut(void * v) const;
};

template <class Ty_>
void * HashList<Ty_>::codeToDst_(Code_ code) const
{
	return  (void *) code;
}

template <class Ty_>
void * HashList<Ty_>::codeToSrc_(Code_ code) const
{
	return (void *) (code >> 32);
}

template <class Ty_>
typename HashList<Ty_>::Code_ HashList<Ty_>::ptrToCode_(void * src, void * dst) const
{
	return (Code_(src) << 32) | Code_(dst);
}

template <class Ty_>
void HashList<Ty_>::insertVertex(void * v)
{
	headsIn_[v] = 0;
	headsOut_[v] = 0;
}

template <class Ty_>
void HashList<Ty_>::insertEdge(void * src, void * dst)
{
	Code_ src_code = ptrToCode_(src, dst);
	Code_ dst_code = ptrToCode_(dst, src);

	nextOut_[src_code] = headsOut_[src];
	nextIn_[dst_code] = headsIn_[dst];
	headsOut_[src] = src_code;
	headsIn_[dst] = dst_code;
}

template <class Ty_>
std::vector<void*> HashList<Ty_>::getAdjacent(void * v) const
{
	std::vector<void*> ret;
	std::unordered_map<void *, Code_>::iterator it = heads_.find(v);
	if (it == heads_.end())
		return ret;
	Code_ head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToDst_(head));
		head = nextIn_[head];
	}

	head = it->second;
	while(head != 0)
	{
		ret.push_back(codeToSrc_(head));
		head = nextOut_[head];
	}

	return ret;
}

template <class Ty_>
std::vector<void*> HashList<Ty_>::getIn(void * v) const
{
	std::vector<void*> ret;
	std::unordered_map<void *, Code_>::const_iterator it = headsIn_.find(v);
	if (it == headsIn_.end())
		return ret;


	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextIn_.find(head)) != nextIn_.end() )
	{
		ret.push_back(codeToDst_(head));
		head = head_it->second;
	}
	
	return ret;
}


template <class Ty_>
std::vector<void*> HashList<Ty_>::getOut(void * v) const
{
	std::vector<void*> ret;
	std::unordered_map<void *, Code_>::const_iterator it = headsOut_.find(v);
	if (it == headsOut_.end())
		return ret;


	Code_ head = it->second;
	std::unordered_map<Code_, Code_>::const_iterator head_it = nextIn_.find(head);
	while( (head_it = nextOut_.find(head)) != nextOut_.end() )
	{
		ret.push_back(codeToDst_(head));
		head = head_it->second;
	}
	
	return ret;
}
