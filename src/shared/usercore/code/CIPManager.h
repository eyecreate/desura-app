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

#ifndef DESURA_CIPMANAGER_H
#define DESURA_CIPMANAGER_H
#ifdef _WIN32
#pragma once
#endif


#include "usercore/CIPManagerI.h"

namespace UserCore
{

	class User;

	namespace Thread
	{
		class MCFThreadI;
	}

	class CIPManager : public UserCore::CIPManagerI
	{
	public:
		CIPManager(gcRefPtr<UserCore::User> user);
		~CIPManager();

		virtual void getCIPList(std::vector<UserCore::Misc::CIPItem> &list);
		virtual void getItemList(std::vector<UserCore::Misc::CIPItem> &list);

		virtual void updateItem(DesuraId id, gcString path);
		virtual void deleteItem(DesuraId id);

		virtual void refreshList();
		virtual bool getCIP(UserCore::Misc::CIPItem& info);

		virtual EventV& getItemsUpdatedEvent();


		gc_IMPLEMENT_REFCOUNTING(CIPManager);

	protected:
		void onRefreshComplete(uint32&);
		void onRefreshError(gcException& e);

		EventV onItemsUpdatedEvent;

	private:
		gcString m_szDBName;
		gcRefPtr<User> m_pUser;

		bool m_bRefreshComplete = false;;
		gcRefPtr<UserCore::Thread::MCFThreadI> m_pThread;
	};
}

#endif //DESURA_CIPMANAGER_H
