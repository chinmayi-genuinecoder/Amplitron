#pragma once

// Neural Amp Modeler (NAM) pedal — loads and runs .nam ML model files
// for realistic amp simulation using RTNeural inference engine.

#include "audio/effects/effect.h"
#include <string>
#include <memory>
#include <mutex>
#include <RTNeural/RTNeural.h>

namespace Amplitron {

/**
 * NAM Loader pedal — loads a .nam model file and runs ML-based
 * amp simulation using the RTNeural inference engine.
 */
class NamLoader : public Effect {
public:
    /** Initializes the pedal with default Level parameter. */
    NamLoader();
    /** Runs NAM inference on the audio buffer in place. */
    void process(float* buffer, int num_samples) override;
    /** Resets model state and clears the loaded model. */
    void reset() override;
    const char* name() const override { return "NAM Loader"; }
    const char* type_id() const override { return "NamLoader"; }
    std::vector<EffectParam>& params() override { return params_; }
    /** Loads a .nam file from the given path. Returns true on success. */
    bool load_model(const std::string& path);
    /** Returns the path of the currently loaded .nam file. */
    const std::string& model_path() const { return model_path_; }

private:
    std::vector<EffectParam> params_;
    std::string model_path_;
    bool model_loaded_ = false;
    std::unique_ptr<RTNeural::Model<float>> model_;
    mutable std::mutex model_mutex_; ///< Protects model state across threads.
};
} // namespace Amplitron

