#pragma once

// Neural Amp Modeler (NAM) pedal — loads and runs .nam ML model files
// for realistic amp simulation using RTNeural inference engine.

#include "audio/effect.h"
#include <string>
#include <memory>
#include <RTNeural/RTNeural.h>

namespace Amplitron {

class NamLoader : public Effect {
public:
    NamLoader();
    void process(float* buffer, int num_samples) override;
    void reset() override;
    const char* name() const override { return "NAM Loader"; }
    const char* type_id() const override { return "NamLoader"; }
    std::vector<EffectParam>& params() override { return params_; }
    bool load_model(const std::string& path);
    const std::string& model_path() const { return model_path_; }

private:
    std::vector<EffectParam> params_;
    std::string model_path_;
    bool model_loaded_ = false;
    std::unique_ptr<RTNeural::Model<float>> model_;
};
} // namespace Amplitron

