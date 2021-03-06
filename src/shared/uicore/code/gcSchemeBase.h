/*
Desura is the leading indie game distribution platform
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)

$LicenseInfo:firstyear=2014&license=lgpl$
Copyright (C) 2014, Linden Research, Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation;
version 2.1 of the License only.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see <http://www.gnu.org/licenses/>
or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
$/LicenseInfo$
*/

#ifndef DESURA_GCSCHEMEBASE_H
#define DESURA_GCSCHEMEBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "cef_desura_includes/ChromiumBrowserI.h"


void RegisterScheme( ChromiumDLL::SchemeExtenderI* scheme );

template <class T>
class RegisterSchemeHelper
{
public:
	RegisterSchemeHelper()
	{
		RegisterScheme( new T() );
	}
};


#define REGISTER_SCHEME( schemeName ) static RegisterSchemeHelper< schemeName > g_RS_##schemeName;


template <class T>
class DesuraSchemeBase : public ChromiumDLL::SchemeExtenderI
{
public:
	DesuraSchemeBase(const char* schemename, const char* hostname)
	{
		m_szHostname = hostname;
		m_szSchemename = schemename;

		m_pCallback = nullptr;
		m_uiResponseSize = 0;
	}

	virtual SchemeExtenderI* clone(const char* scheme)
	{
		return new T();
	}

	virtual void destroy()
	{
		delete this;
	}

	virtual const char* getSchemeName()
	{
		return m_szSchemename.c_str(); //L"desura";
	}

	virtual const char* getHostName()
	{
		return m_szHostname.c_str(); //L"image";
	}

	virtual void registerCallback(ChromiumDLL::SchemeCallbackI* callback)
	{
		m_pCallback = callback;
	}

	virtual size_t getResponseSize()
	{
		return m_uiResponseSize;
	}

	virtual const char* getResponseMimeType()
	{
		if (m_szMimeType.size() == 0)
			return nullptr;

		return m_szMimeType.c_str();
	}

	virtual const char* getRedirectUrl()
	{
		if (m_szRedirectUrl.size() == 0)
			return nullptr;

		return m_szRedirectUrl.c_str();
	}

	virtual ~DesuraSchemeBase(){}

protected:
	size_t m_uiResponseSize;
	gcString m_szMimeType;
	gcString m_szRedirectUrl;

	ChromiumDLL::SchemeCallbackI* m_pCallback;

private:
	gcString m_szHostname;
	gcString m_szSchemename;
};



#endif //DESURA_GCSCHEMEBASE_H
