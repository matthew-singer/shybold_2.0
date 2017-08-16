#include "population.h"
#include <algorithm>
#ifdef OPEN_CV
    #include <opencv2/opencv.hpp>
    #define RADS_TO_DEGREES 57.2958
#endif 
#include "lookup.h"
#include <chrono>

#ifdef OPEN_CV
const int drawX = 000; //draws between 1000 1000 and 2000 2000
const int drawY = 000;
const int size = 1000;

template <class T>
void draw_agent(T &screen, std::shared_ptr<agent> &a, bool isPred ) {
        int x = a->x;
        int y = sizeY - a->y;
        double rSize = isPred ? 8 : 2;
        cv::Scalar colors = isPred ? cv::Scalar(0, 0, 255) : cv::Scalar(255, 0, 0);
        if (x >= drawX && x <= drawX + size) {
            if (y >= drawY && y <= drawY + size) {
                cv::circle(screen, cv::Point(x - drawX, y - drawY), rSize, colors, -1);
                //double radius = std::max(a->g->getRadius(), 0.0);
                //cv::ellipse(screen, cv::Point(x - drawX, y - drawY), cv::Size(radius, radius), -1.0 * pred_a->angle_facing * RADS_TO_DEGREES -1.0 * pred_a->diff_angle * RADS_TO_DEGREES , pred_a->diff_angle * RADS_TO_DEGREES, cv::Scalar(0, 100, 0), -1, 8);
                }
            }
}


#endif 


void population::update() {
    #ifdef OPEN_CV
    cv::Mat screen(size, size, CV_8UC4, cv::Scalar(0xff, 0xff, 0xff));
    #endif
    std::shared_ptr<agent> pred_a;
    std::shared_ptr<agent> prey_a;
    bool writeOut;
    for (int i = 0; i < generations; ++i) { 
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Generation " << i << " of " << generations << '\n';
        for (int reps = 0; reps < replicates; ++reps) {
            //std::shared_ptr<Lookup> lookup_pred = std::make_shared<Lookup>(preds_pop, reps * predator_pop_count, prey_pop_count);
            std::shared_ptr<Lookup> lookup_prey = std::make_shared<Lookup>(prey_pop, reps * prey_pop_count, prey_pop_count);
            
            time = timeTicks;
            while (--time) {
                #ifdef OPEN_CV
                    screen.setTo(0xff);
                #endif

                //N * N loop over pred and prey to update them

                for (int pred_i = reps * predator_pop_count; pred_i < (predator_pop_count * (reps + 1)) ; ++pred_i) {
                    pred_a = preds_pop[pred_i];
                    if (pred_a->alive) {
                        pred_a->reset();
                        //pred_a->input_agent[0] = lookup_prey->valid_agent(sensing_range_pred, pred_a);
                        lookup_prey->valid_agent(sensing_range_pred, pred_a);
                        pred_a->updatePred(timeTicks - time);
                        //pred_a->consume(timeTicks - time);
                        #ifdef OPEN_CV
                            draw_agent(screen, pred_a, true);
                        #endif
                    }
                }
                //
                //N loop over prey to update them 
                #ifdef OPEN_CV
                    for (int prey_i = reps * prey_pop_count; prey_i < prey_pop_count * (reps + 1); ++prey_i) { 
                        prey_a = prey_pop[prey_i];
                        if (prey_a->alive) {
                                draw_agent(screen, prey_a, false);
                        }
                    }
                #endif
            #ifdef OPEN_CV
                #ifdef OPENCV_SAVE
                    if (writeOut) {
                        std::string gen_num = std::to_string(gen);
                        std::string time_num = std::to_string(timeTicks - time);
                        gen_num = std::string(4 - gen_num.length(), '0').append(gen_num); 
                        time_num = std::string(4 -time_num.length(), '0').append(time_num); 
                    
                        cv::imwrite("Gen_" + gen_num + "_Time_" + time_num + ".jpg", screen);
                    }
                #else
                    cv::imshow(windowName, screen);
                    cv::waitKey(8);
                #endif
            #endif 
                     
            } //end of time
            //std::cout << preds_pop[reps * predator_pop_count ]->calcFitnessPred() << '\n'; 
        } //end of replicates
        /*Pred reproduce*/ 
        {
            std::cout << "Predator gamete pool size = ";
            std::vector< std::shared_ptr<chrome> > pred_gametes;
            for (int pred_i = 0; pred_i < predator_pop_count * replicates ; ++pred_i) {
                auto pred_a = preds_pop[pred_i];
                int val = pred_a->calcFitnessPred();
                for (int i = 0; i < val; ++i) {
                    pred_gametes.push_back(pred_a->getGamete());
                }
                pred_a->output_data(output_file_pred, false, i, pred_i);
            }
            reproduce(pred_gametes, preds_pop, predator_pop_count);
        } //used to score and free pred gametes after
        {
            for (int prey_i = 0; prey_i < prey_pop_count * replicates ; ++prey_i) {
                prey_pop[prey_i]->resetLocation();
            } 
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timeTaken = end - start;
        std::cout << timeTaken.count()/1000.0 << " Seconds Taken For generation\n";
    } //end of generations

}

template<std::size_t SIZE>
void population::reproduce(std::vector< std::shared_ptr<chrome> > &gametes, std::array< std::shared_ptr<agent>, SIZE > &pop, int pop_size) {
    //if it is too small just totally replace it
    int gametes_select = 0;
    int gametes_size = gametes.size();

    std::random_shuffle(gametes.begin(), gametes.end());
    std::cout << gametes_size << '\n';
    for (size_t i = 0; i < pop_size * replicates; ++i) {
        /*std::shared_ptr<chrome> p1 = gametes.back();
        gametes.pop_back();
        std::shared_ptr<chrome> p2 = gametes.back();
        gametes.pop_back();*/
        if (gametes_size == 0) {
            pop[i] = std::move(std::make_shared<agent>());
        } else {
            pop[i] = std::move(std::make_shared<agent>(gametes[gametes_select % gametes_size], gametes[(gametes_select + 1) % gametes_size]));
            gametes_select += 2;
            if (gametes_select > gametes_select) {
                gametes_select = 0;
                std::random_shuffle(gametes.begin(), gametes.end());
            }
        }
    }
}

