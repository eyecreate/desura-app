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

#ifndef DESURA_UNINSTALLBRANCHPROCESS_H
#define DESURA_UNINSTALLBRANCHPROCESS_H
#ifdef _WIN32
#pragma once
#endif

#include "util_thread/BaseThread.h"
#include "mcfcore/ProgressInfo.h"

namespace MCFCore{ class MCFI; }

class UninstallBranchProcess : public Thread::BaseThread
{
public:
	UninstallBranchProcess(const char* oldBranchMcf, const char* newBranchMcf, const char* inspath, const char* installScript);
	~UninstallBranchProcess();

	EventV onCompleteEvent;
	EventV onFinishEvent;
	Event<gcException> onErrorEvent;
	Event<uint64> onProgressEvent;

protected:
	void run();
	void onStop();
	void onPause();
	void onUnpause();

	void onError(gcException& e);
	void onProgress(MCFCore::Misc::ProgressInfo& p);

private:
	uint64 m_uiLastProg = 0;

	gcString m_szOldMcfPath;
	gcString m_szNewMcfPath;
	gcString m_szInstallPath;

	gcString m_szInstallScript;

	std::mutex m_McfLock;
	MCFCore::MCFI* m_pOldBranchMcf = nullptr;
	MCFCore::MCFI* m_pNewBranchMcf = nullptr;
};

#endif //DESURA_UNINSTALLBRANCHPROCESS_H
