#include "population.h"
#include <algorithm>

void population::update() {
    
    for (int i = 0; i < generations; ++i) { 
        std::cout << "Generation " << i << " of " << generations << '\n';
        for (int reps = 0; reps < replicates; ++reps) {
            time = timeTicks;
            while (--time) {
                //N * N loop over pred and prey to update them
                for (int prey_i = reps * prey_pop_count; prey_i < prey_pop_count * (reps + 1); ++prey_i) { 
                    prey_pop[prey_i]->reset();
                }

                for (int pred_i = reps * predator_pop_count; pred_i < (predator_pop_count * (reps + 1)) ; ++pred_i) {
                    preds_pop[pred_i]->reset();
                    for (int prey_i = reps * prey_pop_count; prey_i < prey_pop_count * (reps + 1); ++prey_i) { 
                        if (prey_pop[prey_i]->alive) {
                            prey_pop[prey_i]->getNearestAgentPrey(preds_pop[pred_i]);
                            preds_pop[pred_i]->getNearestAgentPred(prey_pop[prey_i]);
                        }
                    }
                    preds_pop[pred_i]->updatePred();
                    preds_pop[pred_i]->consume(timeTicks - time);
                }
                //N loop over prey to update them 
                for (int prey_i = reps * prey_pop_count; prey_i < prey_pop_count * (reps + 1); ++prey_i) { 
                    if (prey_pop[prey_i]->alive) {
                        prey_pop[prey_i]->updatePrey();
                    }
                }
                

            } //end of time
        } //end of replicates
        /*Pred reproduce*/ 
        {
            std::vector< std::shared_ptr<chrome> > pred_gametes;
            for (int pred_i = 0; pred_i < predator_pop_count * replicates ; ++pred_i) {
                int val = preds_pop[pred_i]->calcFitnessPred();
                for (int i = 0; i < val; ++i) {
                    pred_gametes.push_back(preds_pop[pred_i]->getGamete());
                }
                preds_pop[pred_i]->output_data(output_file_pred, false, i, pred_i);
            }
            reproduce(pred_gametes, preds_pop, predator_pop_count);
        } //used to score and free pred gametes after
        /*Prey reproduce*/
        {
            std::vector< std::shared_ptr<chrome> > prey_gametes;
            for (int prey_i = 0; prey_i < prey_pop_count * replicates ; ++prey_i) {
                int val = prey_pop[prey_i]->calcFitnessPrey();
                for (int i = 0; i < val; ++i) {
                    prey_gametes.push_back(prey_pop[prey_i]->getGamete());
                }
                prey_pop[prey_i]->output_data(output_file_prey, true, i, prey_i);
            }
            reproduce(prey_gametes, prey_pop, prey_pop_count);
        } //used to scope and free prey gametes after usage

    } //end of generations

}

template<std::size_t SIZE>
void population::reproduce(std::vector< std::shared_ptr<chrome> > &gametes, std::array< std::shared_ptr<agent>, SIZE > &pop, int pop_size) {
     /*   
    //if it is too small just totally replace it
    if (gametes.size() == 0) {
        for (int i = 0; i < pop_size * 2 + 2; ++i) {
            gametes.push_back(std::shared_ptr<chrome>());
        }
    }

    while ( gametes.size() < 2 * pop_size * replicates ) {
        std::random_shuffle(gametes.begin(), gametes.end());
        gametes.insert(gametes.end(), gametes.begin(), gametes.begin() + gametes.size());
    }
    */
    std::random_shuffle(gametes.begin(), gametes.end());

    for (size_t i = 0; i < pop_size * replicates; ++i) {
        if (gametes.size() > 1) {
            std::shared_ptr<chrome> p1 = gametes.back();
            gametes.pop_back();
            std::shared_ptr<chrome> p2 = gametes.back();
            gametes.pop_back();
            pop[i] = std::move(std::make_shared<agent>(p1, p2));
        } else {
            pop[i] = std::move(std::make_shared<agent>());
        }
    }

}

