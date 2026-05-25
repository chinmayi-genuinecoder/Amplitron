#include "audio/effects/nam_loader.h"
#include "audio/effect_factory.h"

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
        model_loaded_ = false;
        return false;
    }
    model_path_ = path;
    model_loaded_ = true;
    return true;
}

void NamLoader::process(float* buffer, int num_samples) {
    if (!enabled_ || !model_loaded_) return;

    const float level = params_[0].value;

    for (int i = 0; i < num_samples; ++i) {
        buffer[i] = buffer[i] * level;
    }
}

void NamLoader::reset() {
    model_loaded_ = false;
    model_path_.clear();
}

} // namespace Amplitron
