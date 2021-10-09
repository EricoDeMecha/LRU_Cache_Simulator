
#include "CacheStuff.h"

Cache::Cache(CacheInfo& info){
    _info = info;
    addr_info.tag = 32 - this->_info.numByteOffsetBits - this->_info.numSetIndexBits;
    addr_info.setIndex = 0;
}
bool Cache::isHit(const std::bitset<32>& addr){
    uint64_t  index = 0;
    bool identical = true;
    auto _getSetNumber  = [this](const std::bitset<32>& addr){
        std::bitset<28> setNum;
        for(uint64_t  i = _info.numByteOffsetBits, j = 0;
                    i < (_info.numByteOffsetBits + _info.numSetIndexBits);++i, ++j ){
            setNum[j] = addr[i];
        }
        return  setNum.to_ulong();
    };

    uint64_t set_num = _getSetNumber(addr);
    for(index = set_num * _info.associativity; index < (set_num + 1) * _info.associativity; index++){
        if(cache[index][30]){
            identical = check_if_identical(index, addr_info.tag, addr, cache);
            if(identical){
                return true;
            }
        }
    }
    return false;
}

bool Cache::check_if_identical(const uint64_t  &index, const uint &bit_tag,const  std::bitset<32> &lhs, const std::bitset<32> rhs[]){
    for (uint i = 31, j = 28; i > (31 - bit_tag); i--, j--) {
        if (lhs[i] != rhs[index][j]) {
            return false;
        }
    }
    return true;
};

void Cache::HitHandler(const std::bitset<32>& addr){
    auto _getSetNumber  = [this](const std::bitset<32>& addr){
        std::bitset<28> setNum;
        for(uint64_t  i = _info.numByteOffsetBits, j = 0;
            i < (_info.numByteOffsetBits + _info.numSetIndexBits);++i, ++j ){
            setNum[j] = addr[i];
        }
        return  setNum.to_ulong();
    };
    uint64_t  set_num = _getSetNumber(addr);

    for (uint64_t index = set_num * _info.associativity;
         index < (set_num + 1) * _info.associativity; index++) {
        if (cache[index][30] && check_if_identical(index, addr_info.tag, addr, cache)) {
            for (uint64_t j = 0; j < LRU_priority.size(); j++) {
                if (LRU_priority[j] == index) {
                    LRU_priority.erase(LRU_priority.begin() + j);
                    break;
                }
            }
            LRU_priority.push_back(index);
            break;
        }
    }
}

void Cache::subs(const std::bitset<32> &addr) {
    uint64_t  index = findCacheBlockIndex(addr);
    for(uint64_t i = 31, j = 28; j > (31-addr_info.tag); i--, j--){
        cache[index][j] = addr[i];
    }
    cache[index][30] = true;
}

uint64_t Cache::findCacheBlockIndex(const std::bitset<32> &addr) {
    auto GetRandomBlockAddr = [this](const std::bitset<32> &addr){
        auto _getSetNumber  = [this](const std::bitset<32>& addr){
            std::bitset<28> setNum;
            for(uint64_t  i = _info.numByteOffsetBits, j = 0;
                i < (_info.numByteOffsetBits + _info.numSetIndexBits);++i, ++j ){
                setNum[j] = addr[i];
            }
            return  setNum.to_ulong();
        };
        std::random_device rd;
        std::mt19937_64 generator(rd());
        std::uniform_int_distribution<int> unif(0, INT32_MAX);
        auto temp_val = static_cast<uint64_t>(unif(generator)/(INT32_MAX/ _info.associativity + 1));
        uint64_t set_num = _getSetNumber(addr);
        return set_num * _info.associativity + temp_val;
    };

    auto GetLRUBlockAddr = [this](const std::bitset<32>& addr){
        auto _getSetNumber  = [this](const std::bitset<32>& addr){
            std::bitset<28> setNum;
            for(uint64_t  i = _info.numByteOffsetBits, j = 0;
                i < (_info.numByteOffsetBits + _info.numSetIndexBits);++i, ++j ){
                setNum[j] = addr[i];
            }
            return  setNum.to_ulong();
        };
        uint64_t set_num = _getSetNumber(addr);
        for(uint64_t index = set_num * _info.associativity; index < (set_num + 1)*_info.associativity;
                    index++){
            if(!cache[index][30]){
                return index;
            }
        }
        for(uint64_t i = 0;i < LRU_priority.size(); i++){
            for(uint64_t j = set_num * _info.associativity;
            j < (set_num + 1) * _info.associativity; j++){
                if(LRU_priority[i] == j){
                    uint64_t  temp = LRU_priority[j];
                    LRU_priority.erase(LRU_priority.begin() + j );
                    return temp;
                }
            }
        }
        return (uint64_t)0;
    };
    uint64_t cache_addr = 0;
    if(_info.rp == ReplacementPolicy::LRU){/*LRU replacement policy*/
        cache_addr  = GetLRUBlockAddr(addr);
    }else if(_info.rp == ReplacementPolicy::Random){/* random replacement
policy */
        cache_addr = GetRandomBlockAddr(addr);
    }else{
        perror("This replacement policy is not implemented yet\n");
    }
    return cache_addr;
}
