
#include <iostream>
#include <memory>
#include "population.h"
#include <fstream>

std::random_device(rd);
std::mt19937 mutate = std::mt19937(rd());
std::normal_distribution<>mutator = std::normal_distribution<>(mut_mean, mut_stddev);
std::uniform_real_distribution<>rates = std::uniform_real_distribution<>(0, 1);
std::uniform_real_distribution<>rander = std::uniform_real_distribution<>(-8, 8);

unsigned long milliseconds_since_epoch =
    std::chrono::system_clock::now().time_since_epoch() / 
    std::chrono::milliseconds(1);

std::string prey_name = "prey_" + std::to_string(milliseconds_since_epoch) ;
std::string pred_name = "pred_" + std::to_string(milliseconds_since_epoch) ;

std::fstream output_file_prey = std::fstream(prey_name, std::fstream::out);
std::fstream output_file_pred = std::fstream(pred_name, std::fstream::out);

void setup_file(std::fstream &o, std::string oppFile);

int main() {
    
    setup_file(output_file_prey, pred_name);
    setup_file(output_file_pred, prey_name);

    std::unique_ptr<population> pop;
    pop.reset(new population());
    pop->update();


    output_file_prey.close();
    output_file_pred.close();
}
   
void setup_file(std::fstream &o, std::string oppFile) {

    o << "#" << "SisterFile " <<  oppFile << '\n';
    o << "#" << "InputOther " << input << '\n';
    o << "#" << "InputAgents " << input_agents << '\n';
    o << "#" << "Output " << output<< '\n';
    o << "#" << "HiddenLayerSize" << hiddenLayerSize << '\n';
    o << "#" << "HiddenLayers " << hiddenLayers << '\n';
    o << "#" << "GeneNN " << geneNN << '\n';
    o << "#" << "AllGene " << geneCount << '\n';
    o << "#" << "Generations " << generations << '\n';
    o << "#" << "TimeTicks " << timeTicks << '\n';
    o << "#" << "Replicates " << replicates << '\n';
    o << "#" << "PredatorPopCount " << predator_pop_count << '\n';
    o << "#" << "PreyPopCount " << prey_pop_count << '\n';
    o << "#" << "PredatorCapture " << pred_capture << '\n';
    o << "#" << "MutMean " << mut_mean << '\n';
    o << "#" << "MutStddev " << mut_stddev << '\n';
    o << "#" << "MuteRate " << mut_rate << '\n';
    o << "#" << "XoverRate " << xover_rate << '\n';
    o << "#" << "PredFitnessPerPreyCapture " << pred_fitness << '\n';
    o << "#" << "PreyFitnessSurvival " << prey_fitness << '\n';
    o << "Gen" << '\t';
    o << "Index" << '\t';
    o << "Fitness" << '\t';
    for (int i = 0 ; i < geneCount; ++i) {
        o << "Gene" << i << '\t';
    }
    o << "Gametes" << '\n';
}
