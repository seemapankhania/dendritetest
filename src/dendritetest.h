#ifndef DENDRITETEST_H_
#define DENDRITETEST_H_

#include "biodynamo.h"

#include "neuroscience/compile_time_param.h"
#include "neuroscience/neurite_element.h"
#include "neuroscience/neuron_soma.h"

namespace bdm {

  struct dendGrowth : public BaseBiologyModule {
  dendGrowth() : BaseBiologyModule(gAllBmEvents) {}

    template <typename T>
      void Run(T* sim_object) {
        if (sim_object->template IsSoType<NeuriteElement>()) {
          auto&& sim_objectNeurite = sim_object->template ReinterpretCast<NeuriteElement>();
          auto ne = sim_objectNeurite->GetSoPtr();

          ne->ElongateTerminalEnd(10, {0,0,1});

        }

      } // end Run

    ClassDefNV(dendGrowth, 1);
  };

// Define compile time parameter
template <typename Backend>
    struct CompileTimeParam : public DefaultCompileTimeParam<Backend> {
      using NeuriteElement = experimental::neuroscience::NeuriteElement;
      using NeuronSoma = experimental::neuroscience::NeuronSoma;
      using BiologyModules = Variant<dendGrowth>; // add GrowthModule
  };
template <typename TResourceManager = ResourceManager<>>

inline int Simulate(int argc, const char** argv) {
  InitializeBiodynamo(argc, argv);

  Cell cell({0, 0, 0}); // creating the cell at position x, y, z
  cell.SetDiameter(7.5);
  cell.ExtendNewNeurite({0,0,1});
  ResourceManager<>::Get()->push_back(cell);

  // Run simulation
  Scheduler<> scheduler;
  scheduler.Simulate(10);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // DENDRITETEST_H_
