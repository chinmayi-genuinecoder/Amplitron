#include "audio/effects/nam_loader.h"
#include "audio/effects/core/effect_factory.h"

#include <fstream>
#include <stdexcept>

namespace Amplitron {

static EffectRegistrar<NamLoader> reg("NamLoader");

NamLoader::NamLoader() {
    params_ = {
        {"Level", 1.0f, 0.0f, 1.0f, 1.0f, "", "Output level of the NAM model."},
    };
}

bool NamLoader::load_model(const std::string& path) {
    std::ifstream f(path);
    if (!f.good()) {
        std::lock_guard<std::mutex>
        lock(model_mutex_);
        model_.reset();
        model_path_.clear();
        model_loaded_ = false;
        return false;
    }
    try {
        auto temp_model = RTNeural::json_parser::parseJson<float>(f);
        if (!temp_model) {
            std::lock_guard<std::mutex>
            lock(model_mutex_);
            model_.reset();
            model_path_.clear();
            model_loaded_ = false;
            return false;
        }
        temp_model->reset();
        std::lock_guard<std::mutex>
        lock(model_mutex_);
        model_ = std::move(temp_model);
        model_path_ = path;
        model_loaded_ = true;
        return true;
    } catch (...) {
        std::lock_guard<std::mutex>
        lock(model_mutex_);
        model_.reset();
        model_path_.clear();
        model_loaded_ = false;
        return false;
    }
}

void NamLoader::process(float* buffer, int num_samples) {
    if (!enabled_) return;
    std::lock_guard<std::mutex> lock(model_mutex_);
    if (!model_loaded_ || !model_) return;

    const float level = params_[0].value;

    for (int i = 0; i < num_samples; ++i) {
        float input = buffer[i];
        buffer[i] = model_->forward(&input) * level;
    }
}

void NamLoader::reset() {
    std::lock_guard<std::mutex> lock(model_mutex_);
    if (model_) model_->reset();
    model_loaded_ = false;
    model_path_.clear();
}

} // namespace Amplitron
