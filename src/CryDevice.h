#pragma once
#ifndef CRYFS_LIB_CRYDEVICE_H_
#define CRYFS_LIB_CRYDEVICE_H_

#include <messmer/blockstore/interface/BlockStore.h>
#include <messmer/blobstore/interface/BlobStore.h>
#include "CryConfig.h"

#include <boost/filesystem.hpp>
#include <messmer/blockstore/implementations/encrypted/ciphers/AES256_GCM.h>
#include <messmer/fspp/fs_interface/Device.h>

#include "messmer/cpp-utils/macros.h"

namespace cryfs {
class DirBlob;

class CryDevice: public fspp::Device {
public:
  static constexpr uint32_t BLOCKSIZE_BYTES = 32 * 1024;

  using Cipher = blockstore::encrypted::AES256_GCM;

  CryDevice(std::unique_ptr<CryConfig> config, std::unique_ptr<blockstore::BlockStore> blockStore);
  virtual ~CryDevice();

  void statfs(const boost::filesystem::path &path, struct ::statvfs *fsstat) override;

  std::unique_ptr<blobstore::Blob> CreateBlob();
  std::unique_ptr<blobstore::Blob> LoadBlob(const blockstore::Key &key);
  void RemoveBlob(const blockstore::Key &key);

  std::unique_ptr<fspp::Node> Load(const boost::filesystem::path &path) override;

  std::unique_ptr<DirBlob> LoadDirBlob(const boost::filesystem::path &path);

private:
  blockstore::Key GetOrCreateRootKey(CryConfig *config);
  Cipher::EncryptionKey GetOrCreateEncryptionKey(CryConfig *config);
  blockstore::Key CreateRootBlobAndReturnKey();

  std::unique_ptr<blobstore::BlobStore> _blobStore;

  blockstore::Key _rootKey;

  DISALLOW_COPY_AND_ASSIGN(CryDevice);
};

}

#endif
