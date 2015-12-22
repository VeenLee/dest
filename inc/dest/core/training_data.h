/**
 This file is part of Deformable Shape Tracking (DEST).
 
 Copyright Christoph Heindl 2015
 
 Deformable Shape Tracking is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Deformable Shape Tracking is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Deformable Shape Tracking. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEST_TRAINING_DATA_H
#define DEST_TRAINING_DATA_H

#include <dest/core/shape.h>
#include <dest/core/image.h>
#include <vector>
#include <random>

namespace dest {
    namespace core {
        
        struct AlgorithmParameters {
            int numCascades;
            int numTrees;
            int maxTreeDepth;
            int numRandomPixelCoordinates;
            int numRandomSplitTestsPerNode;
            float exponentialLambda;
            float learningRate;
            
            AlgorithmParameters();
        };
        
        struct TrainingData {
            
            struct Sample {
                int idx;
                Shape estimate;
            };
            typedef std::vector<Sample> SampleVector;
            typedef std::vector<Rect> RectVector;
            typedef std::vector<Shape> ShapeVector;
            typedef std::vector<Image> ImageVector;
                        
            RectVector rects;
            ShapeVector shapes;
            ImageVector images;

            SampleVector trainSamples;
            AlgorithmParameters params;
            std::mt19937 rnd;

            static void createTrainingSamplesKazemi(const ShapeVector &shapes, SampleVector &samples, std::mt19937 &rnd, int numInitializationsPerImage = 20);
            static void createTrainingSamplesThroughLinearCombinations(const ShapeVector &shapes, SampleVector &samples, std::mt19937 &rnd, int numInitializationsPerImage = 20);
            static void convertShapesToNormalizedShapeSpace(const RectVector &rects, ShapeVector &shapes);
            static void createTrainingRectsFromShapeBounds(const ShapeVector &shapes, RectVector &rects);
            static void randomPartitionTrainingSamples(SampleVector &train, SampleVector &validate, std::mt19937 &rnd, float validatePercent = 0.1f);
        };
        
        struct RegressorTraining {
            TrainingData *trainingData;
            Shape meanShape;            
            int numLandmarks;
        };
        
        struct TreeTraining {
            struct Sample {
                ShapeResidual residual;
                PixelIntensities intensities;
                
                friend inline void swap(Sample& a, Sample& b)
                {
                    using std::swap;
                    swap(a.residual, b.residual);
                    swap(a.intensities, b.intensities);
                }
            };
            typedef std::vector<Sample> SampleVector;
            
            TrainingData *trainingData;
            SampleVector samples;
            PixelCoordinates pixelCoordinates;
            int numLandmarks;
        };
    }
}

#endif