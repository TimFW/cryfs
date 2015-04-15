#include "Caching2BlockStore.h"
#include "../../interface/Block.h"

using std::unique_ptr;

namespace blockstore {
namespace caching2 {

Caching2BlockStore::Caching2BlockStore(std::unique_ptr<BlockStore> baseBlockStore)
  :_baseBlockStore(std::move(baseBlockStore)) {
}

unique_ptr<Block> Caching2BlockStore::create(size_t size) {
  //TODO Also cache this and only write back in the destructor?
  //     When writing back is done efficiently in the base store (e.g. only one safe-to-disk, not one in the create() and then one in the save(), this is not supported by the current BlockStore interface),
  //     then the base store could actually directly create a block in the create() call, OnDiskBlockStore wouldn't have to avoid file creation in the create() call for performance reasons and I could also adapt the OnDiskBlockStore test cases and remove a lot of flush() calls there because then blocks are loadable directly after the create call() without a flush.
  //     Currently, OnDiskBlockStore doesn't create new blocks directly but only after they're destructed (performance reasons), but this means a newly created block can't be loaded directly.
  return _baseBlockStore->create(size);
}

unique_ptr<Block> Caching2BlockStore::load(const Key &key) {
  return _baseBlockStore->load(key);
}

void Caching2BlockStore::remove(std::unique_ptr<Block> block) {
  return _baseBlockStore->remove(std::move(block));
}

uint64_t Caching2BlockStore::numBlocks() const {
  return _baseBlockStore->numBlocks();
}

}
}
