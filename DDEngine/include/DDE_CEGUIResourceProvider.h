#pragma once

#include <CEGUI/CEGUI.h>
#include <map>
#include <windows.h>

namespace DDEngine
{
class DDE_CEGUIResourceProvider : public CEGUI::ResourceProvider {

	private:
		HMODULE dllHmodule;
		LPSTREAM stream;
		HGLOBAL global;

	public:
		DDE_CEGUIResourceProvider() { }
		DDE_CEGUIResourceProvider(HMODULE hModule) : dllHmodule(hModule) { }
		~DDE_CEGUIResourceProvider();
	
		virtual void loadRawDataContainer(const CEGUI::String& filename,
										CEGUI::RawDataContainer& output, 
										const CEGUI::String& resourceGroup) override;

		virtual size_t getResourceGroupFileNames(std::vector<CEGUI::String>& out_vec,
                                             const CEGUI::String& file_pattern,
                                             const CEGUI::String& resource_group) override;

		void setResourceGroupDirectory(const CEGUI::String& resourceGroup, const CEGUI::String& directory);
		const CEGUI::String& getResourceGroupDirectory(const CEGUI::String& resourceGroup);
		void clearResourceGroupDirectory(const CEGUI::String& resourceGroup);
		void unloadRawDataContainer(CEGUI::RawDataContainer& data);

	protected:
	
		CEGUI::String getFinalFilename(const CEGUI::String& filename, const CEGUI::String& resourceGroup) const;

		typedef std::map<CEGUI::String, CEGUI::String, CEGUI::StringFastLessCompare> ResourceGroupMap;
		ResourceGroupMap d_resourceGroups;

};
}