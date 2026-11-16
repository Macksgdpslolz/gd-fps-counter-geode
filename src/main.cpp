#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>

using namespace geode::prelude;

class $modify(FPSPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* m_fpsLabel = nullptr;
        int m_frameCount = 0;
        std::chrono::steady_clock::time_point m_lastTime = std::chrono::steady_clock::now();
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto label = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        label->setScale(0.4f);
        label->setOpacity(180);
        label->setAnchorPoint({0.f, 1.f});
        label->setPosition({5.f, winSize.height - 5.f});
        label->setID("fps-label"_spr);

        this->addChild(label, 999);
        m_fields->m_fpsLabel = label;
        m_fields->m_lastTime = std::chrono::steady_clock::now();
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (!m_fields->m_fpsLabel) return;

        m_fields->m_frameCount++;
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - m_fields->m_lastTime).count();

        if (elapsed >= 0.25) {
            int fps = static_cast<int>(m_fields->m_frameCount / elapsed + 0.5);
            m_fields->m_fpsLabel->setString(fmt::format("FPS: {}", fps).c_str());
            m_fields->m_frameCount = 0;
            m_fields->m_lastTime = now;
        }
    }
};
