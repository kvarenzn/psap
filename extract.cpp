#include "extract.h"

#include <QBuffer>
#include <QHash>

#include "utils.h"

namespace Extract
{

    const QHash<i32, QString> COMMON_STRINGS{
        {0, "AABB"},
        {5, "AnimationClip"},
        {19, "AnimationCurve"},
        {34, "AnimationState"},
        {49, "Array"},
        {55, "Base"},
        {60, "BitField"},
        {69, "bitset"},
        {76, "bool"},
        {81, "char"},
        {86, "ColorRGBA"},
        {96, "Component"},
        {106, "data"},
        {111, "deque"},
        {117, "double"},
        {124, "dynamic_array"},
        {138, "FastPropertyName"},
        {155, "first"},
        {161, "float"},
        {167, "Font"},
        {172, "GameObject"},
        {183, "Generic Mono"},
        {196, "GradientNEW"},
        {208, "GUID"},
        {213, "GUIStyle"},
        {222, "int"},
        {226, "list"},
        {231, "long long"},
        {241, "map"},
        {245, "Matrix4x4f"},
        {256, "MdFour"},
        {263, "MonoBehaviour"},
        {277, "MonoScript"},
        {288, "m_ByteSize"},
        {299, "m_Curve"},
        {307, "m_EditorClassIdentifier"},
        {331, "m_EditorHideFlags"},
        {349, "m_Enabled"},
        {359, "m_ExtensionPtr"},
        {374, "m_GameObject"},
        {387, "m_Index"},
        {395, "m_IsArray"},
        {405, "m_IsStatic"},
        {416, "m_MetaFlag"},
        {427, "m_Name"},
        {434, "m_ObjectHideFlags"},
        {452, "m_PrefabInternal"},
        {469, "m_PrefabParentObject"},
        {490, "m_Script"},
        {499, "m_StaticEditorFlags"},
        {519, "m_Type"},
        {526, "m_Version"},
        {536, "Object"},
        {543, "pair"},
        {548, "PPtr<Component>"},
        {564, "PPtr<GameObject>"},
        {581, "PPtr<Material>"},
        {596, "PPtr<MonoBehaviour>"},
        {616, "PPtr<MonoScript>"},
        {633, "PPtr<Object>"},
        {646, "PPtr<Prefab>"},
        {659, "PPtr<Sprite>"},
        {672, "PPtr<TextAsset>"},
        {688, "PPtr<Texture>"},
        {702, "PPtr<Texture2D>"},
        {718, "PPtr<Transform>"},
        {734, "Prefab"},
        {741, "Quaternionf"},
        {753, "Rectf"},
        {759, "RectInt"},
        {767, "RectOffset"},
        {778, "second"},
        {785, "set"},
        {789, "short"},
        {795, "size"},
        {800, "SInt16"},
        {807, "SInt32"},
        {814, "SInt64"},
        {821, "SInt8"},
        {827, "staticvector"},
        {840, "string"},
        {847, "TextAsset"},
        {857, "TextMesh"},
        {866, "Texture"},
        {874, "Texture2D"},
        {884, "Transform"},
        {894, "TypelessData"},
        {907, "UInt16"},
        {914, "UInt32"},
        {921, "UInt64"},
        {928, "UInt8"},
        {934, "unsigned int"},
        {947, "unsigned long long"},
        {966, "unsigned short"},
        {981, "vector"},
        {988, "Vector2f"},
        {997, "Vector3f"},
        {1006, "Vector4f"},
        {1015, "m_ScriptingClassIdentifier"},
        {1042, "Gradient"},
        {1051, "Type*"},
        {1057, "int2_storage"},
        {1070, "int3_storage"},
        {1083, "BoundsInt"},
        {1093, "m_CorrespondingSourceObject"},
        {1121, "m_PrefabInstance"},
        {1138, "m_PrefabAsset"},
        {1152, "FileSize"},
        {1161, "Hash128"}
    };

    SerializedType::SerializedType(ByteReader &reader)
    {
        classId = ClassID(reader.get<i32>());
        isStrippedType = reader.get<bool>();
        scriptTypeIndex = reader.get<i16>();

        if (classId == ClassID::MONO_BEHAVIOUR)
        {
            scriptId = reader.read(16);
        }

        oldTypeHash = reader.read(16);

        auto numbersOfNodes = reader.get<i32>();
        auto stringBufferSize = reader.get<i32>();

        for (int i = 0; i < numbersOfNodes; i++)
        {
            auto node = TypeTreeNode();
            node.version = reader.get<u16>();
            node.level = reader.get<i8>();
            node.isArray = reader.get<i8>();
            node.typeStrOffset = reader.get<u32>();
            node.nameStrOffset = reader.get<u32>();
            node.byteSize = reader.get<i32>();
            node.index = reader.get<i32>();
            node.metaFlag = reader.get<i32>();
            node.refTypeHash = reader.get<u64>();
            nodes.append(node);
        }

        auto str = reader.read(stringBufferSize);
        QBuffer stringBuffer(&str);

        const auto readString = [&stringBuffer](i32 offset) -> QString
        {
            if (!(offset & 0x80000000))
            {
                stringBuffer.seek(offset);
                return Utils::readStringToNull(stringBuffer);
            }

            offset = offset & 0x7fffffff;
            return COMMON_STRINGS[offset];
        };

        for (auto &node : nodes)
        {
            node.type = readString(node.typeStrOffset);
            node.name = readString(node.nameStrOffset);
        }

        auto typeDependenciesCount = reader.get<i32>();
        for (int i = 0; i < typeDependenciesCount; i++)
        {
            typeDependencies.append(reader.get<i32>());
        }
    }
}
