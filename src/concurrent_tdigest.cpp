#include "../include/concurrent_tdigest.h"

ConcurrentTDigest::ConcurrentTDigest(double c, size_t mt, size_t bs)
: td_(c, mt), buffer_(), batch_size_(bs)
{
    buffer_.reserve(batch_size_ * 2);
}

void ConcurrentTDigest::add(double value) {
    std::lock_guard<std::mutex> lock(mutex_);
    buffer_.push_back(value);
    if (++count_ >= batch_size_) {
        for (double v : buffer_) td_.add(v);
        buffer_.clear();
        count_ = 0;
    }
}

double ConcurrentTDigest::quantile(double q) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (double v : buffer_) td_.add(v);
    buffer_.clear();
    count_ = 0;
    td_.merge();
    return td_.quantile(q);
}

size_t ConcurrentTDigest::size() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
    return td_.size();
}