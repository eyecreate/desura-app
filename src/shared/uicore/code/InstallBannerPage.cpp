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

#include "Common.h"
#include "InstallBannerPage.h"

#define DEFAULT_BANNER "#icon_download"


namespace UI
{
namespace Forms
{
namespace ItemFormPage
{

InstallBannerPage::InstallBannerPage(wxWindow* parent, bool useSpinnerFirst) : BaseInstallPage(parent)
{
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &InstallBannerPage::onButtonPressed, this);
	Bind(wxEVT_IDLE, &InstallBannerPage::onIdle, this);

	m_labProv = new gcStaticText( this, wxID_ANY, Managers::GetString(L"#IF_PROVIDEDBY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labLabel = new gcStaticText( this, wxID_ANY, Managers::GetString(L"#IF_ETIME"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labInfo = new gcStaticText( this, wxID_ANY, Managers::GetString(L"#IF_INITDOWNLOAD"), wxDefaultPosition, wxDefaultSize, 0 );

	m_pbProgress = new gcSpinnerProgBar(this, wxID_ANY, wxDefaultPosition, wxSize( -1,22 ));
	m_butCancel = new gcButton( this, wxID_ANY, Managers::GetString(L"#CLOSE"), wxDefaultPosition, wxDefaultSize, 0 );

	if (!useSpinnerFirst)
		m_pbProgress->swap();

	m_imgBanner = new gcImageControl( this, wxID_ANY, wxDefaultPosition, wxSize( 358,75 ), 0 );
	m_imgBanner->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVECAPTION ) );
	m_imgBanner->setImage(DEFAULT_BANNER);
	m_imgBanner->SetMinSize( wxSize( 358,75 ) );
	m_imgBanner->SetMaxSize( wxSize( 358,75 ) );
	m_imgBanner->SetCursor(wxCURSOR_HAND);
	m_imgBanner->Bind(wxEVT_LEFT_DOWN, &InstallBannerPage::onMouseClick, this);

	wxFlexGridSizer* fgSizerImg = new wxFlexGridSizer(1, 3, 0, 0);
	fgSizerImg->AddGrowableCol(0);
	fgSizerImg->AddGrowableCol(2);

	fgSizerImg->Add( 0, 0, 1, wxEXPAND, 0);
	fgSizerImg->Add( m_imgBanner, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );
	fgSizerImg->Add( 0, 0, 1, wxEXPAND, 0);

	wxBoxSizer* bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	bSizer17->Add( m_labLabel, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	bSizer17->Add( m_labInfo, 0, wxRIGHT|wxTOP, 5 );
	
	m_pButSizer = new wxBoxSizer( wxHORIZONTAL );
	m_pButSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	m_pButSizer->Add( m_butCancel, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 6, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 2 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	fgSizer1->Add( bSizer17, 0, wxEXPAND, 5 );
	fgSizer1->Add( m_pbProgress, 1, wxEXPAND|wxALL, 5 );
	fgSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	fgSizer1->Add( m_pButSizer, 0, wxEXPAND, 5 );
	fgSizer1->Add( m_labProv, 0, wxLEFT|wxEXPAND, 5 );
	fgSizer1->Add( fgSizerImg, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer1 );
	this->Layout();

	m_bDefaultBanner = true;
	m_iCurProvider = -1;

	hideDownloadBanner();

	gcFrame* par = dynamic_cast<gcFrame*>(GetParent());
	if (par)
		par->setProgressState(gcFrame::P_NORMAL);

	m_tLastBannerChange = gcTime();
}

InstallBannerPage::~InstallBannerPage()
{
}

void InstallBannerPage::onButtonPressed(wxCommandEvent& event)
{
	if (event.GetId() == m_butCancel->GetId())
	{
		GetParent()->Close();
	}
}

void InstallBannerPage::hideDownloadBanner()
{
	m_imgBanner->Show(false);
	m_labProv->Show(false);
	this->setParentSize(-1, 140);
	this->Layout();
}

void InstallBannerPage::showDownloadBanner()
{
	m_imgBanner->Show(true);
	m_labProv->Show(true);
	this->setParentSize(-1, 240);
	this->Layout();
}

void InstallBannerPage::updateProviderList()
{
	if (m_vDownloadProv.size() == 0)
	{
		hideDownloadBanner();
	}
	else
	{
		size_t count = 0;

		for (size_t x=0; x<m_vDownloadProv.size(); x++)
		{
			if (!m_vDownloadProv[x])
				continue;

			count++;
		}

		if (count == 0)
			hideDownloadBanner();
		else
			showDownloadBanner();
	}

	this->Layout();
}

void InstallBannerPage::updateBanner()
{
	if (m_iCurProvider < -1 || !m_vDownloadProv[m_iCurProvider])
		return;

	MCFCore::Misc::DownloadProvider* dp = m_vDownloadProv[m_iCurProvider];

	if (strncmp(dp->getBanner(),"http://",7)==0)
	{
		//download banner
	}
	else
	{
		m_bDefaultBanner = false;
		m_imgBanner->setImage(dp->getBanner());
	}

	Refresh();
}

void InstallBannerPage::onMouseClick( wxMouseEvent& event )
{ 
	if (m_iCurProvider == -1 || !m_vDownloadProv[m_iCurProvider])
		return;

	if (!m_bDefaultBanner)
	{
		gcWString wurl(m_vDownloadProv[m_iCurProvider]->getProvUrl());
		gcLaunchDefaultBrowser(wurl.c_str());
	}
}

void InstallBannerPage::onIdle( wxIdleEvent& event )
{
	size_t size = m_vDownloadProv.size();

	if (size == 0)
		return;

	if (size == 1)
	{
		if (m_iCurProvider < 0)
		{
			m_iCurProvider = 0;
			updateBanner();
		}
	}
	else
	{
		if (m_iCurProvider < 0)
		{
			m_iCurProvider = 0;
			updateBanner();
			m_tLastBannerChange = gcTime();
		}
		else
		{
			auto elasped = gcTime() - m_tLastBannerChange;

			size_t secs = elasped.seconds();

			if (secs > 15)
			{
				for (int32 x=0; x<(int32)size; x++)
				{
					int32 cur = x+m_iCurProvider+1;

					if (cur >= (int32)size)
						cur -= (int32)size;

					if (m_vDownloadProv[cur])
					{
						m_iCurProvider = cur;
						updateBanner();
						m_tLastBannerChange = gcTime();
						break;
					}
				}
			}
		}

	}
}


void InstallBannerPage::onDownloadProvider(UserCore::Misc::GuiDownloadProvider& dp)
{
	if (!dp.provider.isValid())
		return;

	if (dp.action == MCFCore::Misc::DownloadProvider::ADD)
	{
		bool found = false;

		for (size_t x=0; x<m_vDownloadProv.size(); x++)
		{
			if (!m_vDownloadProv[x])
				continue;

			if (strcmp(m_vDownloadProv[x]->getUrl(), dp.provider.getUrl()) == 0)
			{
				delete m_vDownloadProv[x];
				m_vDownloadProv[x] = new MCFCore::Misc::DownloadProvider(dp.provider);
				found = true;
				break;
			}
		}

		if (!found)
			m_vDownloadProv.push_back(new MCFCore::Misc::DownloadProvider(dp.provider));
	}
	else if (dp.action == MCFCore::Misc::DownloadProvider::REMOVE)
	{
		for (size_t x=0; x<m_vDownloadProv.size(); x++)
		{
			if (!m_vDownloadProv[x])
				continue;

			if (strcmp(m_vDownloadProv[x]->getUrl(), dp.provider.getUrl()) == 0)
			{
				delete m_vDownloadProv[x];
				m_vDownloadProv[x] = nullptr;
				break;
			}
		}
	}

	updateProviderList();
}

}
}
}