#include "DDE_CEGUIResourceProvider.h"
#include "DLLResourceLoader.h"
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/Exceptions.h>
#include <CEGUI/ResourceProvider.h>
#include <CEGUI/Base.h>
#include <CEGUI/System.h>
#include <CEGUI/Image.h>
#include <io.h>
#include <stdio.h>

using namespace DDEngine;
using namespace CEGUI;

DDE_CEGUIResourceProvider::~DDE_CEGUIResourceProvider() {

}

#define LOAD_RESOURCE(X) if(filename == X)

void DDE_CEGUIResourceProvider::loadRawDataContainer( const CEGUI::String& filename, 
													 CEGUI::RawDataContainer& output,
													 const CEGUI::String& resourceGroup ) {

	DLLDataContainer dllData;

	LOAD_RESOURCE("AlfiskoSkin.imageset") {
		dllData = DLLResourceLoader::loadCEGUI_imageset(dllHmodule);
	}

	LOAD_RESOURCE("AlfiskoSkin.png") {
		dllData = DLLResourceLoader::loadCEGUI_image(dllHmodule);
	}

	LOAD_RESOURCE("DejaVuSans-10.font") {
		dllData = DLLResourceLoader::loadCEGUI_font(dllHmodule);
	}

	LOAD_RESOURCE("DejaVuSans.ttf") {
		dllData = DLLResourceLoader::loadCEGUI_fontTTF(dllHmodule);
	}

	LOAD_RESOURCE("AlfiskoSkin.looknfeel") {
		dllData = DLLResourceLoader::loadCEGUI_looknfeel(dllHmodule);
	}

	output.setData((CEGUI::uint8*) dllData.dataBlob);
	output.setSize((size_t) dllData.dataSize);
}

void DDE_CEGUIResourceProvider::unloadRawDataContainer(RawDataContainer& data) {
    data.setData(0);
	data.setSize(0);
}

void DDE_CEGUIResourceProvider::setResourceGroupDirectory(
                                                const String& resourceGroup,
                                                const String& directory) {

    if (directory.length() == 0)
        return;

#if defined(_WIN32) || defined(__WIN32__)
    // while we rarely use the unportable '\', the user may have
    const String separators("\\/");
#else
    const String separators("/");
#endif

    if (String::npos == separators.find(directory[directory.length() - 1]))
        d_resourceGroups[resourceGroup] = directory + '/';
    else
        d_resourceGroups[resourceGroup] = directory;
}

const String& DDE_CEGUIResourceProvider::getResourceGroupDirectory(const String& resourceGroup) {
    return d_resourceGroups[resourceGroup];
}

void DDE_CEGUIResourceProvider::clearResourceGroupDirectory(
                                                const String& resourceGroup) {

    ResourceGroupMap::iterator iter = d_resourceGroups.find(resourceGroup);

    if (iter != d_resourceGroups.end())
        d_resourceGroups.erase(iter);
}

size_t DDE_CEGUIResourceProvider::getResourceGroupFileNames( std::vector<CEGUI::String>& out_vec,
															const CEGUI::String& file_pattern, 
															const CEGUI::String& resource_group ) {

	// look-up resource group name
    ResourceGroupMap::const_iterator iter =
        d_resourceGroups.find(resource_group.empty() ? d_defaultResourceGroup :
                              resource_group);
    // get directory that's set for the resource group
    const String dir_name(
        iter != d_resourceGroups.end() ? (*iter).second : "./");

    size_t entries = 0;

    intptr_t f;
    struct _wfinddata_t fd;

    if ((f = _wfindfirst(System::getStringTranscoder().stringToStdWString(
            dir_name + file_pattern).c_str(), &fd)) != -1)
    {
        do
        {
            if ((fd.attrib & _A_SUBDIR))
                continue;

            out_vec.push_back(System::getStringTranscoder().stringFromStdWString(fd.name));
            ++entries;
        }
        while (_wfindnext(f, &fd) == 0);

        _findclose(f);
    }

    return entries;
}

CEGUI::String DDE_CEGUIResourceProvider::getFinalFilename( const String& filename, const String& resourceGroup ) const {
	String final_filename;

    // look up resource group directory
    ResourceGroupMap::const_iterator iter =
        d_resourceGroups.find(resourceGroup.empty() ?
            d_defaultResourceGroup :
            resourceGroup);

    // if there was an entry for this group, use it's directory as the
    // first part of the filename
    if (iter != d_resourceGroups.end())
        final_filename = (*iter).second;

    // append the filename part that we were passed
    final_filename += filename;

    // return result
    return final_filename;
}
