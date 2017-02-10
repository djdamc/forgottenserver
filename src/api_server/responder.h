#ifndef FS_API_SERVER_RESPONDER_H
#define FS_API_SERVER_RESPONDER_H

#include "../tools.h"
#include "common.h"
#include <lua.hpp>

namespace http_api
{

class Peer;
using PeerWeakPtr = std::weak_ptr<Peer>;

class Responder// : NonCopyable
{
	const PeerWeakPtr peerWeak;
	const RequestID requestID;
public:
	const Request request;
	Response response;
	Responder(PeerWeakPtr peer, Request request, RequestID requestID);

	static int luaDelete(lua_State* L);
	static int luaSend(lua_State* L);

	static int luaGetRequestBody(lua_State* L);
	static int luaSetResponseBody(lua_State* L);

	static int luaGetRequestField(lua_State* L);
	static int luaSetResponseField(lua_State* L);

	static int luaGetRequestURL(lua_State* L);
	static int luaGetRequestMethod(lua_State* L);
	static int luaSetResponseStatus(lua_State* L);

	void send();
};

} //namespace http_api

#endif // FS_API_SERVER_RESPONDER_H