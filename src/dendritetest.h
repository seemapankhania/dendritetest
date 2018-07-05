#ifndef DENDRITETEST_H_
#define DENDRITETEST_H_

#include "biodynamo.h"

#include "neuroscience/compile_time_param.h"
#include "neuroscience/neurite_element.h"
#include "neuroscience/neuron_soma.h"

namespace bdm {

using experimental::neuroscience::NeuriteElement;
using experimental::neuroscience::NeuronSoma;

  struct dendGrowth : public BaseBiologyModule {
  dendGrowth() : BaseBiologyModule(gAllBmEvents) {}

    template <typename T>
      void Run(T* sim_object) {
        if (sim_object->template IsSoType<NeuriteElement>()) {
          auto&& sim_objectNeurite = sim_object->template ReinterpretCast<NeuriteElement>();
          auto ne = sim_objectNeurite->GetSoPtr();
          std::array<double, 3> direction = ne->GetSpringAxis();
          ne->ElongateTerminalEnd(10, direction);

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
      using AtomicTypes = VariadicTypedef<NeuronSoma, NeuriteElement>;
  };
template <typename TResourceManager = ResourceManager<>>

inline int Simulate(int argc, const char** argv) {
  Simulation<> simulation(argc, argv);
  auto* rm = simulation.GetResourceManager();
  auto* scheduler = simulation.GetScheduler();

  NeuronSoma soma({0, 0, 0}); // creating the cell at position x, y, z
  soma.SetDiameter(7.5);
  auto neurite1 = soma.ExtendNewNeurite({0,0,1});
  neurite1->AddBiologyModule(dendGrowth());
  auto neurite2 = soma.ExtendNewNeurite({10,10,0});
  neurite2->AddBiologyModule(dendGrowth());
  auto neurite3 =soma.ExtendNewNeurite({20,0,0});
  neurite3->AddBiologyModule(dendGrowth());
  auto neurite4 =soma.ExtendNewNeurite({0,7,0});
  neurite4->AddBiologyModule(dendGrowth());
  rm->Get<NeuronSoma>()->push_back(soma);


  // Run simulation
  scheduler->Simulate(100);

  std::cout << "Simulation completed successfully!" << std::endl;
  return 0;
}

} // namespace bdm

#endif // DENDRITETEST_H_
