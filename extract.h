#ifndef EXTRACT_H
#define EXTRACT_H

#include <QString>
#include <QList>

#include "bytereader.hpp"

namespace Extract {
using u64 = unsigned long long;
using u32 = unsigned int;
using u16 = unsigned short;
using u8 = unsigned char;

using i64 = signed long long;
using i32 = signed int;
using i16 = signed short;
using i8 = signed char;

enum ClassID {
    OBJECT = 0,
    GAME_OBJECT,
    TRANSFORM = 4,
    MATERIAL = 21,
    TEXTURE_2D = 28,
    SHADER = 48,
    TEXT_ASSET,
    ANIMATION_CLIP = 74,
    AUDIO_CLIP = 83,
    ANIMATOR_CONTROLLER = 91,
    ANIMATOR = 95,
    MONO_BEHAVIOUR = 114,
    MONO_SCRIPT,
    FONT = 128,
    ASSET_BUNDLE = 142,
    SPRITE_RENDERER = 212,
    SPRITE,
    CANVAS_RENDERER = 222,
    CANVAS,
    RECT_TRANSFORM
};

struct StorageBlock {
    u32 uncompressedSize;
    u32 compressedSize;
    u16 flags;
};

struct Node {
    i64 offset;
    i64 size;
    u32 flags;
    QString path;
};

struct TypeTreeNode {
    QString type;
    QString name;
    i32 byteSize;
    i32 index;
    bool isArray;
    i32 version;
    i32 metaFlag;
    i32 level;
    u32 typeStrOffset;
    u32 nameStrOffset;
    u64 refTypeHash;
};

struct SerializedType {
    ClassID classId;
    bool isStrippedType;
    i16 scriptTypeIndex = -1;
    QList<TypeTreeNode> nodes;
    QByteArray scriptId;
    QByteArray oldTypeHash;
    QList<i32> typeDependencies;
    QString className;
    QString nameSpace;
    QString asmName;

    SerializedType(ByteReader &reder);
};

struct ObjectInfo {
    i64 byteStart;
    u32 byteSize;
    i32 typeId;
    i32 classId;
    u16 isDestroyed;
    i8 stripped;

    i64 pathId;
    SerializedType serializedType;
};

struct LocalSerializedObjectIdentifier {
    i32 localSerializedFileIndex;
    i64 localIdentifierInFile;
};

struct FileIdentifier {
    QByteArray guid;
    i32 type;
    QString pathName;
    QString fileName;
};

enum SerializedFileFormatVersion {
    kUnsupported = 1,
    kUnknown_2,
    kUnknown_3,
    kUnknown_5 = 5,
    kUnknown_6,
    kUnknown_7,
    kUnknown_8,
    kUnknown_9,
    kUnknown_10,
    kHasScriptTypeIndex,
    kUnknown_12,
    kHasTypeTreeHashes,
    kUnknown_14,
    kSupportsStrippedObject,
    kRefactoredClassId,
    kRefactorTypeData,
    kRefactorShareableTypeTreeData,
    kTypeTreeNodeWithTypeFlags,
    kSupportsRefObject,
    kStoresTypeDependencies,
    kLargeFilesSupport
};

struct SerializedFileHeader {
    u32 metadataSize;
    i64 fileSize;
    SerializedFileFormatVersion version;
    i64 dataOffset;
    i8 endianess;
    QByteArray reserved;
};

struct SerializedFile {
    QString fullName;
    QString originalPath;
    QString filename;

    i32 version[4];
};

}
#endif // EXTRACT_H
