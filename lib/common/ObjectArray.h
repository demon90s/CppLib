#pragma once

#include <cstdlib>
#include <list>
#include <stdexcept>

// 对象数组, 泛型, 随机访问, 动态增长, 预分配内存并回收利用
// Add(v) 添加一个元素, 返回其下标
// Remove(index) 使用下标删除一个元素
// Exist(index) 该下标的元素是否存在
// Size() 返回元素的个数
// Capacity() 返回已分配内存的个数
// Clear() 删除所有元素
// operator[] 获取下标上的元素, 不存在抛出 out of range 异常

// 缺陷: 
// - 只能用迭代器来遍历数组, 使用 for (; i < Size(); i++) 形式则可能抛出 out of range 异常

// 优点:
// - 插入 删除 查询的最佳效率为 O(1)

template<typename T>
class ObjectArray {
    struct E {
        T v;
        bool constructed;
    };

    template<typename Value, typename Arr = ObjectArray>
    struct Iterator {
        Iterator(Arr* object_arr) : index_(0), object_arr_(object_arr) {}
        Iterator(Arr* object_arr, int index) : index_(0), object_arr_(object_arr) {}

        bool operator==(const Iterator<Value, Arr>& rhs) {
            return index_ == rhs.index_ && object_arr_ == rhs.object_arr_;
        }
        bool operator!=(const Iterator<Value, Arr>& rhs) {
            return !(*this == rhs);
        }
        Value& operator*() {
            return (*object_arr_)[index_];
        }
        Value* operator->() {
            return &this->operator*();
        }
        Iterator<Value, Arr>& operator++() {
			if (index_ < object_arr_->Capacity())
				++index_;

            while (!object_arr_->Exist(index_) && index_ < object_arr_->Capacity()) {
                ++index_;
            }
            return *this;
        }
        Iterator<Value, Arr> operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }
        
        int index_;
        Arr *object_arr_;
    };
public:
    ObjectArray(int reserve = 1) : reserve_(reserve), arr_(nullptr), beg_(this), end_(this) {
        arr_ = (E*)malloc(sizeof(E) * reserve_);
        for (int i = 0; i < reserve_; i++) {
            arr_[i].constructed = false;
            reuse_index_list_.push_back(i);
        }
    }
    ~ObjectArray() {
        if (arr_) {
            this->Clear();
            free(arr_);
            arr_ = nullptr;
        }
    }
    ObjectArray(const ObjectArray& oa);
    ObjectArray& operator=(ObjectArray oa) {
        oa.swap(*this);
        return *this;
    }

    void swap(ObjectArray &oa) {
        std::swap(reserve_, oa.reserve_);
        std::swap(arr_, oa.arr_);
        reuse_index_list_.swap(oa.reuse_index_list_);
        std::swap(beg_, oa.beg_);
        std::swap(end_, oa.end_);
    }

    int Size() const {
        return reserve_ - reuse_index_list_.size();
    }

    int Capacity() const {
        return reserve_;
    }

    void Clear() {
        reuse_index_list_.clear();
        for (int i = 0; i < reserve_; i++) {
            if (arr_[i].constructed) {
                arr_[i].v.~T();
				arr_[i].constructed = false;
            }
            reuse_index_list_.push_back(i);
        }
    }

    int Add(const T &v) {
        this->CheckIncrease();

        int index = reuse_index_list_.front();
        new (&arr_[index].v)T(v);
		arr_[index].constructed = true;
        reuse_index_list_.pop_front();

        if (index >= end_.index_)
            end_.index_ = index + 1;
        
        return index;
    }

    void Remove(int index) {
        if (index >= 0 && index < reserve_) {
            if (arr_[index].constructed) {
                arr_[index].v.~T();
                arr_[index].constructed = false;

                reuse_index_list_.push_back(index);
            }
        }
    }

    bool Exist(int index) const {
        if (index >= 0 && index < reserve_) {
            if (arr_[index].constructed) {
                return true;
            }
        }
        return false;
    }

    Iterator<T> Find(int index) {
        if (!Exist(index)) {
            return end_;
        }
        return Iterator<T>(this, index);
    }

    T& operator[](int index) {
        if (!this->Exist(index)) {
            throw std::out_of_range("ObjectArray out of range");
        }
        return arr_[index].v;
    }

    const T& operator[](int index) const {
        if (!this->Exist(index)) {
            throw std::out_of_range("ObjectArray out of range");
        }
        return arr_[index].v;
    }

    Iterator<T> begin() { return beg_; }
    Iterator<T> end() { return end_; }

private:
    void CheckIncrease();

    int reserve_;
    E *arr_;
    std::list<int> reuse_index_list_;

    Iterator<T> beg_;
    Iterator<T> end_;
};

template<typename T>
ObjectArray<T>::ObjectArray(const ObjectArray& oa): reserve_(oa.reserve_), arr_(nullptr), beg_(this), end_(this)
{
    arr_ = (E*)malloc(sizeof(E) * reserve_);
    for (int i = 0; i < reserve_; i++) {
        if (oa.Exist(i)) {
            arr_[i].v = oa.arr_[i].v;
            arr_[i].constructed = true;
        }
        else {
            arr_[i].constructed = false;
            reuse_index_list_.push_back(i);
        }
    }
    end_.index_ = reserve_;
}

template<typename T>
void ObjectArray<T>::CheckIncrease()
{
    if (reuse_index_list_.size() > 0)
        return;
    int new_capacity = reserve_ * 2;
    std::list<int> new_resue_index_list;
    E *tmp_arr = (E*)malloc(sizeof(E) * new_capacity);
    if (tmp_arr) {
        for (int i = 0; i < new_capacity; i++) {
            if (i >= reserve_) {
                tmp_arr[i].constructed = false;
                new_resue_index_list.push_back(i);
            }
            else {
                tmp_arr[i].v = arr_[i].v;
                tmp_arr[i].constructed = true;
            }
        }

        this->Clear();
        free(arr_);
        arr_ = tmp_arr;
        reserve_ = new_capacity;
        reuse_index_list_ = std::move(new_resue_index_list);
        end_.index_ = reserve_;
    }
}
