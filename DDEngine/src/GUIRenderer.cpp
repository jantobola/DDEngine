#include "GUIRenderer.h"
#include "RenderContext.h"
#include "Config.h"
#include "DLLResourceLoader.h"
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Direct3D11/Renderer.h>

using namespace DDEngine;
using namespace CEGUI;

GUIRenderer::GUIRenderer(Config& config, RenderContext& renderContext) : config(config), renderContext(renderContext) {
	resourcesHmodule = LoadLibrary(L"dderes.dll");
	ceguiResourceProvider = DDE_CEGUIResourceProvider(resourcesHmodule);
	initialize();
}

GUIRenderer::~GUIRenderer() {
	cleanUp();
	FreeLibrary(resourcesHmodule);
}

void GUIRenderer::initialize() {

	// BOOTSTRAP MANUAL
	guiRenderer = &Direct3D11Renderer::create(renderContext.device, renderContext.context);
	System::performVersionTest(CEGUI_VERSION_ABI, 208, CEGUI_FUNCTION_NAME);
	if (System::getSingletonPtr()) CEGUI_THROW(InvalidRequestException("CEGUI::System object is already initialised."));
	System::create(*guiRenderer, &ceguiResourceProvider);
	// BOOTSTRAP MANUAL

	WindowManager& windowManager = WindowManager::getSingleton();
	CEGUI::Window* rootWindow = windowManager.createWindow( "DefaultWindow", "root" );
	System::getSingleton().getDefaultGUIContext().setRootWindow( rootWindow );

	ImageManager::setImagesetDefaultResourceGroup("imagesets");
	Font::setDefaultResourceGroup("fonts");
	Scheme::setDefaultResourceGroup("schemes");
	WidgetLookManager::setDefaultResourceGroup("looknfeels");
	WindowManager::setDefaultResourceGroup("layouts");
	
	DataContainer dllData = DLLResourceLoader::loadCEGUI_scheme(resourcesHmodule);

	RawDataContainer cScheme;
	cScheme.setData((CEGUI::uint8*) dllData.dataBlob);
	cScheme.setSize((size_t) dllData.dataSize);

	SchemeManager::getSingleton().createFromContainer(cScheme);

	cScheme.setData(0);
	cScheme.setSize(0);
}

void GUIRenderer::render() {
	CEGUI::System::getSingleton().renderAllGUIContexts();
}

void GUIRenderer::cleanUp() {
	System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    Direct3D11Renderer* renderer =
        static_cast<Direct3D11Renderer*>(sys->getRenderer());

    System::destroy();
    guiRenderer->destroy(*guiRenderer);
}