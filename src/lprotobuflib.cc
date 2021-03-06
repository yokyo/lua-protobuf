#include "lprotobuflib.h"
#include "google/protobuf/reflection.h" 
#include "google/protobuf/map.h" 
#include "google/protobuf/descriptor.pb.h" 
#include <lua.hpp>

#define KEY(field) (field->message_type()->field(0))
#define VALUE(field) (field->message_type()->field(1))

using namespace google::protobuf;

static void protobuf_serialize(lua_State *L, Message* message)
{
  size_t bytes = message->ByteSizeLong();
  char* data = new char[bytes];

  if (!message->SerializeToArray(data, bytes))
  {
    delete [] data;
    luaL_error(L, "failed to serialize");
  }

  lua_pushlstring(L, data, bytes);
  delete [] data;
}

static void protobuf_parse(lua_State *L, int index, Message* message)
{
  size_t bytes;
  const char* data = luaL_checklstring(L, index, &bytes);

  if (!message->ParseFromArray(data, bytes))
  {
    luaL_error(L, "failed to parse");
  }
}

static void protobuf_tomessage(lua_State *L, int index, Message* message)
{
  luaL_checktype(L, index, LUA_TTABLE);
  const Descriptor* descriptor = message->GetDescriptor();
  const Reflection* reflection = message->GetReflection();
  int count = descriptor->field_count();

  for (int i = 0; i < count; i++)
  {
    const FieldDescriptor* field = descriptor->field(i);
    int type = lua_getfield(L, index, field->name().c_str());

    if (type != LUA_TNIL)
    {
      switch (field->type())
      {
        case FieldDescriptor::TYPE_DOUBLE:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not a number");
              reflection->AddDouble(message, field, lua_tonumber(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not a number");
            reflection->SetDouble(message, field, lua_tonumber(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_FLOAT:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not a number");
              reflection->AddFloat(message, field, lua_tonumber(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not a number");
            reflection->SetFloat(message, field, lua_tonumber(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_INT32:
        case FieldDescriptor::TYPE_SINT32:
        case FieldDescriptor::TYPE_SFIXED32:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not an integer");
              reflection->AddInt32(message, field, lua_tointeger(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not an integer");
            reflection->SetInt32(message, field, lua_tointeger(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_INT64:
        case FieldDescriptor::TYPE_SINT64:
        case FieldDescriptor::TYPE_SFIXED64:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not an integer");
              reflection->AddInt64(message, field, lua_tointeger(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not an integer");
            reflection->SetInt64(message, field, lua_tointeger(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_UINT32:
        case FieldDescriptor::TYPE_FIXED32:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not an integer");
              reflection->AddUInt32(message, field, lua_tointeger(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not an integer");
            reflection->SetUInt32(message, field, lua_tointeger(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_UINT64:
        case FieldDescriptor::TYPE_FIXED64:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not an integer");
              reflection->AddUInt64(message, field, lua_tointeger(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not an integer");
            reflection->SetUInt64(message, field, lua_tointeger(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_BOOL:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TBOOLEAN, field->number(), "not a boolean");
              reflection->AddBool(message, field, lua_toboolean(L, -1));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TBOOLEAN, field->number(), "not a boolean");
            reflection->SetBool(message, field, lua_toboolean(L, -1));
          }
          break;

        case FieldDescriptor::TYPE_STRING:
        case FieldDescriptor::TYPE_BYTES:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TSTRING, field->number(), "not a string");
              reflection->AddString(message, field, std::string(lua_tostring(L, -1)));
              lua_pop(L, 1);
            }
          }
          else
          {
            luaL_argcheck(L, type == LUA_TSTRING, field->number(), "not a string");
            reflection->SetString(message, field, std::string(lua_tostring(L, -1)));
          }
          break;

        case FieldDescriptor::TYPE_ENUM:
          if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
#ifdef ENUM_AS_NUMBER
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TNUMBER, field->number(), "not a number");
              lua_Integer number = lua_tointeger(L, -1);
              const EnumValueDescriptor *value = field->enum_type()->FindValueByNumber(number);
#else
              luaL_argcheck(L, lua_rawgeti(L, -1, n) == LUA_TSTRING, field->number(), "not a string");
              const char *name = lua_tostring(L, -1);
              const EnumValueDescriptor *value = field->enum_type()->FindValueByName(name);
#endif
              luaL_argcheck(L, value != NULL, field->number(), "unknown enum");
              reflection->AddEnum(message, field, value);
              lua_pop(L, 1);
            }
          }
          else
          {
#ifdef ENUM_AS_NUMBER
            luaL_argcheck(L, type == LUA_TNUMBER, field->number(), "not a number");
            lua_Integer number = lua_tointeger(L, -1);
            const EnumValueDescriptor *value = field->enum_type()->FindValueByNumber(number);
#else
            luaL_argcheck(L, type == LUA_TSTRING, field->number(), "not a string");
            const char *name = lua_tostring(L, -1);
            const EnumValueDescriptor *value = field->enum_type()->FindValueByName(name);
#endif
            luaL_argcheck(L, value != NULL, field->number(), "unknown enum");
            reflection->SetEnum(message, field, value);
          }
          break;

        case FieldDescriptor::TYPE_MESSAGE:
          if (field->message_type()->options().map_entry())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            const MutableRepeatedFieldRef<Message> entries = reflection->GetMutableRepeatedFieldRef<Message>(message, field);
            lua_pushnil(L);

            while (lua_next(L, -2) != 0)
            {
              Message* entry = entries.NewMessage();

              switch (KEY(field)->type())
              {
                case FieldDescriptor::TYPE_INT32:
                case FieldDescriptor::TYPE_SINT32:
                case FieldDescriptor::TYPE_SFIXED32:
                  luaL_checktype(L, -2, LUA_TNUMBER);
                  entry->GetReflection()->SetInt32(entry, KEY(field), lua_tointeger(L, -2));
                  break;

                case FieldDescriptor::TYPE_INT64:
                case FieldDescriptor::TYPE_SINT64:
                case FieldDescriptor::TYPE_SFIXED64:
                  luaL_checktype(L, -2, LUA_TNUMBER);
                  entry->GetReflection()->SetInt64(entry, KEY(field), lua_tointeger(L, -2));
                  break;

                case FieldDescriptor::TYPE_UINT32:
                case FieldDescriptor::TYPE_FIXED32:
                  luaL_checktype(L, -2, LUA_TNUMBER);
                  entry->GetReflection()->SetUInt32(entry, KEY(field), lua_tointeger(L, -2));
                  break;

                case FieldDescriptor::TYPE_UINT64:
                case FieldDescriptor::TYPE_FIXED64:
                  luaL_checktype(L, -2, LUA_TNUMBER);
                  entry->GetReflection()->SetUInt64(entry, KEY(field), lua_tointeger(L, -2));
                  break;

                case FieldDescriptor::TYPE_BOOL:
                  luaL_checktype(L, -2, LUA_TBOOLEAN);
                  entry->GetReflection()->SetBool(entry, KEY(field), lua_toboolean(L, -2));
                  break;

                case FieldDescriptor::TYPE_STRING:
                  luaL_checktype(L, -2, LUA_TSTRING);
                  entry->GetReflection()->SetString(entry, KEY(field), std::string(lua_tostring(L, -2)));
                  break;

                default:
                  luaL_error(L, "unsupported key: %s", KEY(field)->type_name());
              }

              switch (VALUE(field)->type())
              {
                case FieldDescriptor::TYPE_DOUBLE:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetDouble(entry, VALUE(field), lua_tonumber(L, -1));
                  break;

                case FieldDescriptor::TYPE_FLOAT:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetFloat(entry, VALUE(field), lua_tonumber(L, -1));
                  break;

                case FieldDescriptor::TYPE_INT32:
                case FieldDescriptor::TYPE_SINT32:
                case FieldDescriptor::TYPE_SFIXED32:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetInt32(entry, VALUE(field), lua_tointeger(L, -1));
                  break;

                case FieldDescriptor::TYPE_INT64:
                case FieldDescriptor::TYPE_SINT64:
                case FieldDescriptor::TYPE_SFIXED64:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetInt64(entry, VALUE(field), lua_tointeger(L, -1));
                  break;

                case FieldDescriptor::TYPE_UINT32:
                case FieldDescriptor::TYPE_FIXED32:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetUInt32(entry, VALUE(field), lua_tointeger(L, -1));
                  break;

                case FieldDescriptor::TYPE_UINT64:
                case FieldDescriptor::TYPE_FIXED64:
                  luaL_checktype(L, -1, LUA_TNUMBER);
                  entry->GetReflection()->SetUInt64(entry, VALUE(field), lua_tointeger(L, -1));
                  break;

                case FieldDescriptor::TYPE_BOOL:
                  luaL_checktype(L, -1, LUA_TBOOLEAN);
                  entry->GetReflection()->SetBool(entry, VALUE(field), lua_toboolean(L, -1));
                  break;

                case FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::TYPE_BYTES:
                  luaL_checktype(L, -1, LUA_TSTRING);
                  entry->GetReflection()->SetString(entry, VALUE(field), std::string(lua_tostring(L, -1)));
                  break;

                case FieldDescriptor::TYPE_ENUM:
                  {
#ifdef ENUM_AS_NUMBER
                    lua_Integer number = luaL_checkinteger(L, -1);
                    const EnumValueDescriptor *value = VALUE(field)->enum_type()->FindValueByNumber(number);
#else
                    const char *name = luaL_checkstring(L, -1);
                    const EnumValueDescriptor *value = VALUE(field)->enum_type()->FindValueByName(name);
#endif
                    luaL_argcheck(L, value != NULL, -1, "unknown enum");
                    entry->GetReflection()->SetEnum(entry, VALUE(field), value);
                  }
                  break;

                case FieldDescriptor::TYPE_MESSAGE:
                  protobuf_tomessage(L, lua_gettop(L), entry->GetReflection()->MutableMessage(entry, VALUE(field)));
                  break;

                default:
                  luaL_error(L, "unsupported value: %s", VALUE(field)->type_name());
              }

              entries.Add(*entry);
              lua_pop(L, 1);
            }
          }
          else if (field->is_repeated())
          {
            luaL_argcheck(L, type == LUA_TTABLE, field->number(), "not a table");
            const MutableRepeatedFieldRef<Message> entries = reflection->GetMutableRepeatedFieldRef<Message>(message, field);
            int len = luaL_len(L, -1);

            for (int n = 1; n <= len; n++)
            {
              Message* entry = entries.NewMessage();
              lua_rawgeti(L, -1, n);
              protobuf_tomessage(L, lua_gettop(L), entry);
              entries.Add(*entry);
              lua_pop(L, 1);
            }
          }
          else
          {
            protobuf_tomessage(L, lua_gettop(L), reflection->MutableMessage(message, field));
          }

          break;

        default:
          luaL_error(L, "unsupported field: %s", field->type_name());
      }
    }

    lua_pop(L, 1);
  }
}

static void protobuf_pushmessage(lua_State *L, const Message& message)
{
  const Descriptor* descriptor = message.GetDescriptor();
  const Reflection* reflection = message.GetReflection();

  int count = descriptor->field_count();
  lua_createtable(L, 0, count);

  for (int i = 0; i < count; i++)
  {
    const FieldDescriptor* field = descriptor->field(i);
    lua_pushstring(L, field->name().c_str());

    switch (field->type())
    {
      case FieldDescriptor::TYPE_DOUBLE:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushnumber(L, reflection->GetRepeatedDouble(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushnumber(L, reflection->GetDouble(message, field));
        }
        break;

      case FieldDescriptor::TYPE_FLOAT:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushnumber(L, reflection->GetRepeatedFloat(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushnumber(L, reflection->GetFloat(message, field));
        }
        break;

      case FieldDescriptor::TYPE_INT32:
      case FieldDescriptor::TYPE_SINT32:
      case FieldDescriptor::TYPE_SFIXED32:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushinteger(L, reflection->GetRepeatedInt32(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushinteger(L, reflection->GetInt32(message, field));
        }
        break;

      case FieldDescriptor::TYPE_INT64:
      case FieldDescriptor::TYPE_SINT64:
      case FieldDescriptor::TYPE_SFIXED64:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushinteger(L, reflection->GetRepeatedInt64(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushinteger(L, reflection->GetInt64(message, field));
        }
        break;

      case FieldDescriptor::TYPE_UINT32:
      case FieldDescriptor::TYPE_FIXED32:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushinteger(L, reflection->GetRepeatedUInt32(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushinteger(L, reflection->GetUInt32(message, field));
        }
        break;

      case FieldDescriptor::TYPE_UINT64:
      case FieldDescriptor::TYPE_FIXED64:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushinteger(L, reflection->GetRepeatedUInt64(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushinteger(L, reflection->GetUInt64(message, field));
        }
        break;

      case FieldDescriptor::TYPE_BOOL:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushboolean(L, reflection->GetRepeatedBool(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushboolean(L, reflection->GetBool(message, field));
        }
        break;

      case FieldDescriptor::TYPE_STRING:
      case FieldDescriptor::TYPE_BYTES:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            lua_pushstring(L, reflection->GetRepeatedString(message, field, index).c_str());
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
          lua_pushstring(L, reflection->GetString(message, field).c_str());
        }
        break;

      case FieldDescriptor::TYPE_ENUM:
        if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
#ifdef ENUM_AS_NUMBER
            lua_pushinteger(L, reflection->GetRepeatedEnum(message, field, index)->number());
#else
            lua_pushstring(L, reflection->GetRepeatedEnum(message, field, index)->name().c_str());
#endif
            lua_rawseti(L, -2, index + 1);
          }
        }
        else
        {
#ifdef ENUM_AS_NUMBER
          lua_pushinteger(L, reflection->GetEnum(message, field)->number());
#else
          lua_pushstring(L, reflection->GetEnum(message, field)->name().c_str());
#endif
        }
        break;

      case FieldDescriptor::TYPE_MESSAGE:
        if (field->message_type()->options().map_entry())
        {
          const RepeatedFieldRef<Message> entries = reflection->GetRepeatedFieldRef<Message>(message, field);
          lua_createtable(L, 0, entries.size());

          for (RepeatedFieldRef<Message>::const_iterator it = entries.begin(); it != entries.end(); it++)
          {
            const Message& entry = *it;

            switch (KEY(field)->type())
            {
              case FieldDescriptor::TYPE_INT32:
              case FieldDescriptor::TYPE_SINT32:
              case FieldDescriptor::TYPE_SFIXED32:
                lua_pushinteger(L, entry.GetReflection()->GetInt32(entry, KEY(field)));
                break;

              case FieldDescriptor::TYPE_INT64:
              case FieldDescriptor::TYPE_SINT64:
              case FieldDescriptor::TYPE_SFIXED64:
                lua_pushinteger(L, entry.GetReflection()->GetInt64(entry, KEY(field)));
                break;

              case FieldDescriptor::TYPE_UINT32:
              case FieldDescriptor::TYPE_FIXED32:
                lua_pushinteger(L, entry.GetReflection()->GetUInt32(entry, KEY(field)));
                break;

              case FieldDescriptor::TYPE_UINT64:
              case FieldDescriptor::TYPE_FIXED64:
                lua_pushinteger(L, entry.GetReflection()->GetUInt64(entry, KEY(field)));
                break;

              case FieldDescriptor::TYPE_BOOL:
                lua_pushboolean(L, entry.GetReflection()->GetBool(entry, KEY(field)));
                break;

              case FieldDescriptor::TYPE_STRING:
                lua_pushstring(L, entry.GetReflection()->GetString(entry, KEY(field)).c_str());
                break;

              default:
                luaL_error(L, "unsupported key: %s", KEY(field)->type_name());
            }

            switch (VALUE(field)->type())
            {
              case FieldDescriptor::TYPE_DOUBLE:
                lua_pushnumber(L, entry.GetReflection()->GetDouble(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_FLOAT:
                lua_pushnumber(L, entry.GetReflection()->GetFloat(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_INT32:
              case FieldDescriptor::TYPE_SINT32:
              case FieldDescriptor::TYPE_SFIXED32:
                lua_pushinteger(L, entry.GetReflection()->GetInt32(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_INT64:
              case FieldDescriptor::TYPE_SINT64:
              case FieldDescriptor::TYPE_SFIXED64:
                lua_pushinteger(L, entry.GetReflection()->GetInt64(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_UINT32:
              case FieldDescriptor::TYPE_FIXED32:
                lua_pushinteger(L, entry.GetReflection()->GetUInt32(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_UINT64:
              case FieldDescriptor::TYPE_FIXED64:
                lua_pushinteger(L, entry.GetReflection()->GetUInt64(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_BOOL:
                lua_pushboolean(L, entry.GetReflection()->GetBool(entry, VALUE(field)));
                break;

              case FieldDescriptor::TYPE_STRING:
              case FieldDescriptor::TYPE_BYTES:
                lua_pushstring(L, entry.GetReflection()->GetString(entry, VALUE(field)).c_str());
                break;

              case FieldDescriptor::TYPE_ENUM:
#ifdef ENUM_AS_NUMBER
                lua_pushinteger(L, entry.GetReflection()->GetEnum(entry, VALUE(field))->number());
#else
                lua_pushstring(L, entry.GetReflection()->GetEnum(entry, VALUE(field))->name().c_str());
#endif
                break;

              case FieldDescriptor::TYPE_MESSAGE:
                protobuf_pushmessage(L, entry.GetReflection()->GetMessage(entry, VALUE(field)));
                break;

              default:
                luaL_error(L, "unsupported value: %s", VALUE(field)->type_name());
            }

            lua_rawset(L, -3);
          }
        }
        else if (field->is_repeated())
        {
          int size = reflection->FieldSize(message, field);
          lua_createtable(L, 0, size);

          for (int index = 0; index < size; index++)
          {
            protobuf_pushmessage(L, reflection->GetRepeatedMessage(message, field, index));
            lua_rawseti(L, -2, index + 1);
          }
        }
        else if (reflection->HasField(message, field))
        {
          protobuf_pushmessage(L, reflection->GetMessage(message, field));
        }
        else
        {
          lua_pushnil(L);
        }
  
        break;

      default:
        luaL_error(L, "unsupported field: %s", field->type_name());
    }

    lua_rawset(L, -3);
  }
}

static int protobuf_encode(lua_State *L)
{
  const char* name = luaL_checkstring(L, 1);
  const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(name);
  luaL_argcheck(L, descriptor != NULL, 1, "message type not found");
  Message* message = MessageFactory::generated_factory()->GetPrototype(descriptor)->New();
  protobuf_tomessage(L, 2, message);
  protobuf_serialize(L, message);
  return 1;
}

static int protobuf_decode(lua_State *L)
{
  const char* name = luaL_checkstring(L, 1);
  const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(name);
  luaL_argcheck(L, descriptor != NULL, 1, "message type not found");
  Message* message = MessageFactory::generated_factory()->GetPrototype(descriptor)->New();
  protobuf_parse(L, 2, message);
  protobuf_pushmessage(L, *message);
  return 1;
}

static const struct luaL_Reg protobuf_f [] = {
  {"encode", protobuf_encode},
  {"decode", protobuf_decode},
  {NULL, NULL}
};

LUAOPEN_PROTOBUF(SUFFIX)
{
  luaL_newlib(L, protobuf_f);
  return 1;
}
