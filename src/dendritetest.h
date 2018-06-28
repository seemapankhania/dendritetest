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
        if (sim_object->template IsSoType<experimental::neuroscience::NeuriteElement>()) {
          auto&& sim_objectNeurite = sim_object->template ReintepretCast<experimental::neuroscience::NeuriteElement>;
          auto neurite = sim_objectNeurite.GetSoPtr();


        }

        // code to be executed each at simulation step
      }

    ClassDefNV(dendGrowth, 1);
  };
// Define compile time parameter
template <typename Backend>
    struct CompileTimeParam : public DefaultCompileTimeParam<Backend> {
      // using NeuronSoma = typename TCompileTimeParam::NeuronSoma;
      using NeuronSoma = experimental::neuroscience::NeuronSoma;
      using BiologyModules = Variant<dendGrowth>; // add GrowthModule
  };
template <typename TResourceManager = ResourceManager<>>

inline int Simulate(int argc, const char** argv) {
  InitializeBiodynamo(argc, argv);

//  Param::min_bound_ = 0;
//  Param::max_bound_ = 100; // cube of 100*100*100

  auto rm = TResourceManager::Get(); // set up resource manager
  auto cells = rm->template Get<Cell>(); // create a structure to contain cells
  cells->reserve(1); // allocate the correct number of cell in our cells structure before cell creation

  Cell cell({0, 0, 0}); // creating the cell at position x, y, z
  cell.SetDiameter(7.5);
//  cell.ExtendNewNeurite({0,0,1});
  cells->push_back(cell); // put the created cell in our cells structure
  cells->Commit();

  // Run simulation for one timestep
  Scheduler<> scheduler;
  scheduler.Simulate(1);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // DENDRITETEST_H_
