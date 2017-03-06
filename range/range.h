#ifndef RANGE_H_
#define RANGE_H_

class range {
public:
	class iterator {
		friend class range;
	public:
		long int operator*() const {
			return _i;
		}
		const iterator& operator++() {
			++_i;
			return *this;
		}
		iterator operator++(int) {
			iterator copy(*this);
			++_i;
			return copy;
		}

		bool operator==(const iterator& other) const {
			return _i == other._i;
		}
		bool operator!=(const iterator& other) const {
			return _i != other._i;
		}
	protected:
		iterator(long int start):
			_i(start)
		{}
	private:
		unsigned long _i;
	};

	explicit range(long int begin, long int end):
		_begin(begin), _end(end)
	{}
	explicit range(long int end):
		range(0, end)
	{}

	iterator begin() const {
		return _begin;
	}
	iterator end() const {
		return _end;
	}
private:
	iterator _begin;
	iterator _end;
};

#endif
