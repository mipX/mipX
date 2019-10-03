/*=========================================================================
 
 Program:   mipx
 Module:    mxSkeleton.h
 
 Authors: Danilo Babin, Hrvoje Leventic.
 Copyright (c) Danilo Babin, Hrvoje Leventic.
 All rights reserved.
 See Copyright.txt
 
 Licensed under the BSD License 2.0.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notice for more information.
 
 =========================================================================*/



#if defined(mxSkeleton_USE_SOURCE_CODE) || defined(mxCore_USE_SOURCE_CODE)
    #define mxSkeleton_API
#else
    #if defined(_MSC_VER)
        #ifdef mxSkeleton_EXPORTS
            #define mxSkeleton_API __declspec(dllexport)
        #else
            #define mxSkeleton_API __declspec(dllimport)
        #endif
    #else
        #ifdef mxSkeleton_EXPORTS
            #define mxSkeleton_API __attribute__((visibility("default")))
        #else
            #define mxSkeleton_API
        #endif
    #endif
#endif



#ifndef mxSkeleton_H
    #define mxSkeleton_H



#include "mxList.h"
#include "mxObject.h"
#include "mxPoint.h"
#include "mxVoxel.h"
#include "mxGeometry.h"
#include "mxImage.h"
#include "mxPointList.h"

#include <string.h>



// Forward declarations.
class mxSkeleton;
class mxSkeleton3D;
class mxSkeletonNode;


#define mxSkeleton_POINT_PRECISION 0.005


/// Skeleton Link contains positions in the path (branch), connections to 2 nodes, skeletonization voxels, metric...

class mxSkeleton_API mxSkeletonLink
{
private:

	/// This function is used in normal sampling. 
	/// CAUTION! It works ONLY for number_of_samples<=2*GetNumberOfPositions().
	int SimpleUpSamplingOfPath(int number_of_samples);

	/// Pointer to the skeleton 3D that this link belongs to.
	mxSkeleton3D *m_skeleton3D;

	/// Pointer to the skeleton to which this link belongs.
	mxSkeleton *m_skeleton;

	/// This value is used for best path calculation. 
	/// It will be added to the metric values of nodes when the information is propagated.
	double m_cost;

	/// List of positions in the link - these are the values this class works on and modifies.
	mxList< mxPoint > m_positions;

	/// List of voxels in the link - these are the values obtained as the output of the 
	/// skeletonization algorithm. This list is maintained to be able to map the modified 
	/// skeleton to the original skeletonization result and hence segmentation.
	mxList< mxVoxel > m_skeletonization_voxels;

	/// Pointers to two nodes that this link connects
	mxSkeletonNode *m_node1;
	mxSkeletonNode *m_node2;


public:

	/// Constructor.
	mxSkeletonLink();
    
    /// Destructor.
	~mxSkeletonLink();
	
	/// Reset the object to an initial state.
	void Reset();
    
    /// Get node of the link closest to 'input_position'.
    mxSkeletonNode* GetNodeCloserToInputPosition(mxPoint &input_position);
    
    /// This is the list of links of nodes of this link (except this link).
    void GetAdjacentLinks(mxList<mxSkeletonLink*> &adjacent_links);

    /// Get average value of specified scalars in the link.
    int GetScalarAverageValue(unsigned int scalar_index, double &output_average);
    
    /// Get min and max value of specified scalars in the link.
    int GetScalarMinAndMaxValue(unsigned int scalar_index, double &output_min, double &output_max);
    
    /// Puts all pointers to positions that define a link into an array (includes start and end node positions).
    /// Warning: these are pointers to positions in the link, hence they are LOCAL positions.
    int GetArrayOfPositionPointersIncludingNodes(mxArray<mxPoint*> &output_array_of_position_pointers);
    
    /// Create an array of link positions in WORLD coordinates (NOT local). Array will include start and end node world positions.
    int GetArrayOfWorldPositionsIncludingNodes(mxArray<mxPoint> &output_array_of_world_positions);

    /// Get node1 position. If none set, return NULL.
    mxPoint* GetNode_1_Position();
    
    /// Get node1. If none set, return NULL.
    mxSkeletonNode* GetNode_1() { return m_node1; };
    
    /// Get node2 position. If none set, return NULL.
    mxPoint* GetNode_2_Position();
    
    /// Get node2. If none set, return NULL.
    mxSkeletonNode* GetNode_2() { return m_node2; };

	/// Get the pointer to the skeleton containing this link.
    mxSkeleton* GetSkeleton() { return m_skeleton; };

	/// Get the pointer to the 3D skeleton containing this link.
    mxSkeleton3D* GetSkeleton3D() { return m_skeleton3D; };

	/// Get pointer to the list of positions in the link.
    mxList< mxPoint >* GetPositionsList() { return &m_positions; };

	/// Get number of positions in the link.
    int GetNumberOfPositions() { return m_positions.GetNumberOfElements(); };

	/// Get cost of the link.
    double& GetCost() { return m_cost; };

	/// Get skeletonization voxels.
    mxList< mxVoxel >* GetSkeletonizationVoxels() { return &m_skeletonization_voxels; };
    
    /// Pring info.
	std::ostream& Print(std::ostream &o);
	
    /// Check if pointers to nodes are not NULL (is they have been set). Success 1, fail 0.
	int IsConnectedToNodes();

	/// Checks if the link is equal in positions of nodes, containing points and skeletonization voxels.
    /// Note: in the current implementation links are not equal if the order of positions is inverse (although the positions are the same)
	int IsEqualTo(mxSkeletonLink *psl);

	/// Check of the list of positions is empty.
	int IsListOfPositionsEmpty();

	/// Check if the link is a loop- if it's connected on both sides to the same node.
	int IsLoop();

	/// Check if the link is a stub- if one of it's nodes has only onle link.
	int IsStub();

    /// Check if the LOCAL position is on link.
    int IsLocalPositionOnLink(mxPosition &local_position);
    
    /// Check if the WORLD position is on link.
    int IsWorldPositionOnLink(mxPosition &world_position);

	/// Makes a correction in the link positions, so that there are no multiple (equal by coordinates) positions entered in 
	/// the link and that no entered link positions are the same as the start_node or end_node position.
	void CorrectLink();

	/// Remove the given position from the link. If the position is not deleted (not found or any other reason) returns 0, if success return 1.
	/// NOTICE: If there are multiple (equal by coordinates) positions, it only deletes the first one it finds.
	int RemovePosition(mxPoint &position_to_remove);

	/// Switch link orientation, which means that the link is inversed - The start node becomes end node, 
	/// end node becomes start node, and the positions are put in the inverse order.
	int InvertPositionOder();

	/// Calculates the length of the link (in mm) from the START node to the END node!
	double LengthOfLink();

	/// Get the bounding box index coordinates for the given link. CAUTION: Start and End Node are also taken into account!
	int GetBounds(int &s_start_included, int &s_end_included, int &r_start_included, int &r_end_included, int &c_start_included, int &c_end_included);
    
    /// Set pointer to node 1.
    void SetNode_1(mxSkeletonNode *node1) { m_node1 = node1; };
    
    /// Set pointer to node 2.
    void SetNode_2(mxSkeletonNode *node2) { m_node2 = node2; };
    
    /// Set the container skeleton.
    void SetSkeleton(mxSkeleton *skeleton) { m_skeleton = skeleton;};

    /// Set the container 3D skeleton.
    void SetSkeleton3D(mxSkeleton3D *skeleton3D) { m_skeleton3D = skeleton3D;};

	/// Smooth the path of the link with the input coefficients. For current position P[i] the new position is:		
	/// P[i]_new = ( P[i-3]*prev_3 + P[i-2]*prev_2 + P[i-1]*prev_1 + P[i] + P[i+1]*next_1 + P[i+2]*next_2 + P[i+3]*next_3 ) / n;
	/// where n is the sum of all the coefficients+1 for the coefficients involved in calculation (neighboring positions taken into account).
	/// Node positions are the only ones not affected by smoothing. If any of the previous(next) 3 positions is the node position,
	/// its effect in smoothing is calculated as e.g.: prev_x*P[i-x]*node_point_strength, where P[i-x] is node position. This insures 
	/// convergence of the smoothed path towards the nodes depending on the node_point_strength coefficient. If you want no special convergence
    /// set it to: node_point_strength = 1. Note: scalar values are not modified.
	int Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength);

	/// Samples the path with the given number of samples, so the resulting link will have 'number_of_samples' positions. It is not equidistant
	/// sampling - the samples are taken by using the existing ones and by adding some interpolated positions at certain places in the path.
	int Sample(int number_of_samples);

	/// Paste POSITIONS connected with lines in the skeleton link to an existing image with scalars given by input index.
	/// NOTICE: PASTING OF SCALARS IS NOW DONE FROM A POSITION TO NEXT POSITION WITH THE SAME VALUE, NOT HALF WAY (AS IT SHOULD BE)!
	int PasteToExisting3DImage(mxImage &output_binary_skeleton_image, unsigned int scalar_index=0);
	
	/// Loads link positions from string of positions, the first read position is put in 'output_start_node_position', the last read position 
	/// is put in 'output_end_node_position'. Returns 1 if succeeded, 0 if failed. !!! IT DOES NOT SET POINTERS TO NODES !!!
	int LoadFromStringOfPositions(const char *positions_string, mxPoint &output_start_node_position, mxPoint &output_end_node_position);

	/// Remove all the positions and skeletonization voxels from this link, but the link itself remains with existing connections to nodes.
	void RemoveAllPositionsAndSkeletonizationVoxels();

};




//--------------------------------------------------------------------------------------------------------------------------



/// Skeleton Node is defined by a single position, contains list of connecting links, skeletonization voxels, metric...

class mxSkeleton_API mxSkeletonNode
{ 
private:

	/// mxPoint of the node.
	mxPoint m_position;

	/// List of links connected to the node.
	mxList<mxSkeletonLink*> m_links;

	/// List of discrete points in the node - these are the values obtained as the output of the skeletonization algorithm, and are the
	/// points from which the coordinate of this node was detemined by averaging the coordinated. This list is maintained to be able to 
	/// map the modified skeleton to the original skeletonization result and hence segmentation.
	mxList<mxVoxel> m_skeletonization_voxels;

	/// Pointer to the skeleton 3D that this link belongs to.
	mxSkeleton3D *m_skeleton3D;

	/// Pointer to the skeleton to which this link belongs.
	mxSkeleton *m_skeleton;
	
	/// Following values are used for calculation of best path in skeleton.
	/// This is the "distance" of the node from the start (convergence) node.
	double m_metric_value;
	
	/// The 'metric_value_2' is used for converging the graph to another point after the first one 
	/// (which uses 'metric_value'). This is used so that a number of best paths can be calculated (not just one). 
	/// This is done by searching all nodes of the skeleton and taking a desired 
	/// number of those that have the smallest 'metric_value'+'metric_value_2' value. 
	double m_metric_value_2;

	/// In best path calculation, these are used to indicate the link towards the source node.
	mxSkeletonLink *m_link_to_source_node;
	mxSkeletonLink *m_link_to_source_node_2;//used in combination with 'metric_value_2'.
	

public:

	/// Constructor.
	mxSkeletonNode();

	/// Destructor.
	~mxSkeletonNode();

	/// Reset the object to the initial state (just after the constructor).
	void Reset();

	/// Get number of links connected to the node.
	int GetNumberOfLinks();

	/// Get node position.
	mxPoint& GetPosition();

	/// Get list of links connected to the node.
	mxList<mxSkeletonLink*>* GetLinksList();
    
    /// Get container skeleton.
    mxSkeleton* GetSkeleton() {return m_skeleton;};

    /// Get container skeleton.
    mxSkeleton3D* GetSkeleton3D() {return m_skeleton3D;};
    
	/// Get list of skeletonization voxels.
	mxList<mxVoxel>* GetSkeletonizationVoxels();

	/// Get metric values of the node (used in best path calculation).
	double& GetMetricValue();
	double& GetMetricValue_2();

	/// Get link to source node (used in best path calculation).
	mxSkeletonLink* GetLinkToSourceNode();
	mxSkeletonLink* GetLinkToSourceNode_2();

	/// Print info.
	std::ostream& Print(std::ostream &o);

	/// Loads node 'node_skeletonization_points' from string of points. Returns 1 if success, 0 if fail.
	int LoadFromStringOfPoints(char *points_string);

	/// Used for converging the "graph" when calculating best path. It updates the 'metric_value' by assigning priority 
	/// to HIGHER values found in links. (The propagated value is gathered from neighboring nodes and links between them 
	/// and this node). If value updated, return 1, else 0.
	int GetUpdates_PropagateHigherValue();

	/// Used for converging the "graph" when calculating best path. It updates the 'metric_value' by assigning priority 
	/// to SMALLER values found in links. (The propagated value is gathered from neighboring nodes and links between them 
	/// and this node). If value updated, return 1, else 0.
	int GetUpdates_PropagateLowerValue();

	/// Used for converging the "graph" when calculating best path. It updates the 'metric_value' by assigning priority 
	/// to HIGHER values found in links. (The propagated value is gathered from neighboring nodes and links between them 
	/// and this node). If value updated, return 1, else 0. WORKS ON metric_value_2!
	int GetUpdates_PropagateHigherValue_2();

	/// Used for converging the "graph" when calculating best path. It updates the 'metric_value' by assigning priority 
	/// to SMALLER values found in links. (The propagated value is gathered from neighboring nodes and links between them 
	/// and this node). If value updated, return 1, else 0. WORKS ON metric_value_2!
	int GetUpdates_PropagateLowerValue_2();
    
    /// Set link to source node (used in best path calculation).
    void SetLinkToSourceNode(mxSkeletonLink *l) { m_link_to_source_node = l; };
    void SetLinkToSourceNode_2(mxSkeletonLink *l) { m_link_to_source_node_2 = l; };
    
    /// Set the container skeleton.
    void SetSkeleton(mxSkeleton *skeleton) { m_skeleton = skeleton;};
    
    /// Set the container 3D skeleton.
    void SetSkeleton3D(mxSkeleton3D *skeleton3D) { m_skeleton3D = skeleton3D;};

};




//----------------------------------------------------------------------------------------------------------------------------



/// Skeleton3D as graph-type 3D structure consisting of a list of links and a list of nodes.

class mxSkeleton_API mxSkeleton3D
{
public:

	/// Lists of nodes and links.
	mxList<mxSkeletonLink> m_links;
	mxList<mxSkeletonNode> m_nodes;

	/// Convergence nodes used in best path calculation.
	mxSkeletonNode *m_convergence_node1, *m_convergence_node2;//The 2 convergence points

	/// Constructor.
	mxSkeleton3D();

	/// Destructor.
	~mxSkeleton3D();

	/// Reset object to initial state (just ofter construction).
	void Reset();

	/// Check if the skeleton 3D is empty.
	int IsEmpty();

	/// Check if the skeleton 3D contains at least one link (number_of_links>=1 and number_of_nodes>=2).
	int HasAtLeastOneLink();

	/// Get number of links.
	unsigned int GetNumberOfLinks();
	
	/// Get number of nodes.
	unsigned int GetNumberOfNodes();

	/// Print basic info.
	std::ostream& PrintHeader(std::ostream &o);
};



//----------------------------------------------------------------------------------------------------------------------------



/// Helper class to map scalar labels to indexes

class mxSkeleton_API mxSkeletonScalarTagElement
{
public:
    
    /// Tag name.
    mxString m_tag;
    
    /// Value 0 means unused, otherwise used.
    int m_is_used;
    
    /// For visualization purposes. Tells the range of scalars that needs to be visualized.
    /// If lower bound value is greater than higher bound value, the whole scalars range should be used.
    double m_scalar_visualization_range[2];
    
    /// Constructor.
    mxSkeletonScalarTagElement()
    {
        m_is_used = 0;
        m_tag.Assign("");
        m_scalar_visualization_range[0] = 1;
        m_scalar_visualization_range[1] = 0;
    };
    
    /// Destructor.
    ~mxSkeletonScalarTagElement() {};
    
    void Copy(mxSkeletonScalarTagElement *ste)
    {
        if(!ste) return;
        m_is_used = ste->m_is_used;
        m_tag.Assign(ste->m_tag);
        m_scalar_visualization_range[0] = ste->m_scalar_visualization_range[0];
        m_scalar_visualization_range[1] = ste->m_scalar_visualization_range[1];
    };
    
    /// Get scalar tag.
    mxString* GetTag() { return &m_tag; };
    
    /// Get visualization range.
    double* GetVisualizationRange() { return m_scalar_visualization_range; };
    
    /// Check if the scalar field is being used.
    int IsUsed() { return m_is_used; };

    /// Check if the visualization range is set.
    int IsVisualizationRangeSet()
    {
        if(m_scalar_visualization_range[1]>m_scalar_visualization_range[0]) return 1;
        return 0;
    };

    
    /// Reset the object to the initial state (just after construction).
    void Reset()
    {
        m_is_used = 0;
        m_tag.Assign("");
        m_scalar_visualization_range[0] = 1;
        m_scalar_visualization_range[1] = 0;
    };
};





/// Skeleton as graph-type 4D structure of multiple 3D skeletons.

class mxSkeleton_API mxSkeleton : public mxDataObject
{
public:

    /// Origin C,R,S,T
	double m_origin[4];
    
    /// Spacing C,R,S,T
	double m_spacing[4];

	/// Array of time instances containing 3D skeleton structures (these are the time series).
	mxArray< mxSkeleton3D > m_data;

//	/// List of names for added scalars.
//	mxList< mxString > m_scalar_tags;
//	mxList< unsigned int > m_scalar_tags_indexes;
    
    /// Array of scalar names and indexes.
    mxSkeletonScalarTagElement m_scalars[mxPoint_NUMBER_OF_VALUES];

//    /// The maximum number of scalars that can be constained in links (defined by maximum number of values in mxPoint).
//    int m_maximum_number_of_scalars;



	/// Constructor.
	mxSkeleton();

	/// Destructor.
	~mxSkeleton();

	/// Reset the object to its initial state (just ofter construction).
	virtual void Reset();

	/// Reset the object for the given time index.
	virtual void ResetTimeIndex(unsigned int t);

	/// Check if the skeleton is empty.
	int IsEmpty();

	/// Set the number of time series of skeleton.
	int SetNumberOfTimeSeries(unsigned int number_of_time_series);

	/// Get the number of timeseries of the skeleton.
	unsigned int GetNumberOfTimeSeries();

	/// For an input link find out to which time index it belongs. Success return 1, fail 0.
	int TimeIndexForLink(mxSkeletonLink *input_link, unsigned int &output_time_index);
    
    /// Re-implemented from bdDataObject to get visual bounds of the obejct.
    int Bounds_3D_WorldCoordinates(double &x_min, double &x_max, double &y_min, double &y_max, double &z_min, double &z_max);

	/// Check if the skeleton contains at least one link (number_of_links>=1 and number_of_nodes>=2).
	int HasAtLeastOneLink();

	/// Get pointer to the list of links.
	mxList<mxSkeletonLink>* GetLinks(unsigned int t=0);

	/// Get pointer to the list of nodes
	mxList<mxSkeletonNode>* GetNodes(unsigned int t=0);

	/// Get pointer to convergence node (used in finding the best path).
	mxSkeletonNode* GetConvergenceNode(unsigned int t=0);
	mxSkeletonNode* GetConvergenceNode_2(unsigned int t=0);// pointer to the second convergence node.
    
    /// Get maximum number of scalars that can be entered.
    unsigned int GetMaximumNumberOfScalars() { return mxPoint_NUMBER_OF_VALUES; };

	/// Get total number of links (for all time indexes).
	unsigned int GetNumberOfLinksInTotal();

	/// Get number of links for the given time index.
	unsigned int GetNumberOfLinks(unsigned int t=0);
	
	/// Get total number of nodes (for all time indexes).
	unsigned int GetNumberOfNodesInTotal();

	/// Get number of nodes for the given time index.
	unsigned int GetNumberOfNodes(unsigned int t=0);

	/// Get scalar tag element.
    mxSkeletonScalarTagElement* GetScalarTagElement(unsigned int i) { return &(m_scalars[i]); };

	/// Get pointer to the list of scalar tag indexes.
	mxList<unsigned int>* GetScalarTagIndexes();

//    /// Get number of entered scalars (and thier tags).
//    unsigned int GetNumberOfScalars();
//
//    /// Get maximum number of scalars that we can store in a single position of a link or node. This value
//    /// is defined by limitation set in mxPoint.
//    unsigned int GetMaximumNumberOfScalars();

	/// Get/Set origin.
	double* GetOrigin();
	double GetOrigin_T();
	double GetOrigin_S();
	double GetOrigin_R();
	double GetOrigin_C();
	void SetOrigin(double t, double s, double r, double c);
	void SetOrigin(double s, double r, double c);

	/// Get/Set spacing.
	double* GetSpacing();
	double GetSpacing_T();
	double GetSpacing_S();
	double GetSpacing_R();
	double GetSpacing_C();
	int SetSpacing(double t, double s, double r, double c);
	int SetSpacing(double s, double r, double c);

    
    /// Convert input world position to local skeleton position.
    void LocalPositionFromWorldPosition(double world_x, double world_y, double world_z, double &output_local_c, double &output_local_r, double &output_local_s);
    
    /// Convert input local skeleton position to world position.
    void WorldPositionFromLocalPosition(double local_c, double local_r, double local_s, double &output_world_x, double &output_world_y, double &output_world_z);

	/// Set the visualization properties (origin, spacing) to match the properties of input skeleton.
	void SetVisualizationPropertiesAs(mxSkeleton &input);

	/// For an input tag get its index in the mxPoint objects. Use this to get index for sequential access
	/// to the specific scalars (defined by input tag). Success 1, fail 0.
	int GetScalarTagIndex(mxString &tag, unsigned int &output_index);

	/// Add a new scalar tag. Success 1, fail 0.
	int AddScalarTag(mxString &tag, unsigned int &output_index);
	
	///Create a new link (sets the pointer to the skeleton in the link to this object).
	mxSkeletonLink* AddLink(unsigned int t=0);
	mxSkeletonLink* AddLink(mxSkeleton3D *skeleton_3D);

	/// Number of stub links (ones that are dead-ends).
	int NumberOfStubLinks(unsigned int t=0);

	/// Check if the number of stub links is greater than a given number (needed for efficient WHILE and FOR loops).
	int IsNumberOfStubLinksGreaterThan(int n, unsigned int t=0);

	/// Check if the number of stub nodes or free nodes (without any links) is greater than a given number (needed for efficient WHILE and FOR loops).
	int IsNumberOfStubOrFreeNodesGreaterThan(int n, unsigned int t=0);

	/// Calls CopyFrom() method.
	mxSkeleton& operator =(mxSkeleton &s);

	/// Set properties (spacing, origin) to be equal to the input properties.
	void SetPropertiesAs(mxSkeleton &input);

	/// Set scalar tags to be equal to the tags of the input skeleton.
	void SetScalarTagsAs(mxSkeleton &input);

	/// Searches for an EQUIVALENT link (so, not by address, but by contents). Found returns 1, fail 0.
	int IsEquivalentLinkInSkeleton(mxSkeletonLink *psl, unsigned int t=0);// former: IsLinkInThisSkeleton(mxSkeletonLink *psl);

	/// Searches for the link with given address in the skeleton. Found returns 1, fail 0.
	int IsLinkInSkeleton(mxSkeletonLink *psl, unsigned int t=0);

	/// Searches for the node with given address in the skeleton. Found returns 1, fail 0.
	int IsNodeInSkeleton(mxSkeletonNode *psn, unsigned int t=0);

	/// Check if there is any link that connects the given 2 nodes.
	int IsLinkConnectingGivenNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2);

	/// Check if there is a path that connects the given 2 nodes in this skeleton. Found returns 1, fail 0.
	int IsPathConnectingGivenNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2);

	/// This is used when shrinking the skeleton. It checks if all the links are connected together in a single 
	/// node (so, if there is one node that contains all possible links, while other nodes are stubs- they have only one link).
	int IsEveryLinkConnectedToOneNode(unsigned int t=0);

	///Finds the bounds of the skeleton and stores into given positions, so the ranges will be: [min.s,max.s], [min.r,max.r], [min.c,max.c].
	int GetBounds(mxPoint &min, mxPoint &max, unsigned int t=0);

	/// Get the link to which the port belongs. PORT is the first or the last position in the list of link positions. Success 1, fail 0.
	int GetLinkPointerForGivenPort(mxPoint &port, mxSkeletonLink **output_link_pointer, unsigned int t=0);

	/// If multiple links connect 2 nodes, find the link that has the highest metric value. If no link found, return 0.
	int GetLinkWithHighestMetricConnetingTwoNodes(mxSkeletonNode *psn1, mxSkeletonNode *psn2, mxSkeletonLink **output_psl);

	/// Get the pointer to the node with given position. If node not found, returns 0. If found, returns 1.
	int GetNodeForPosition(mxPoint &node_position, mxSkeletonNode **output_node_pointer, unsigned int t=0);

	/// Get the pointer to the node that contains the most links of all the nodes. 
	int GetNodeWithTheMostLinks(mxSkeletonNode **output_node_pointer, unsigned int t=0);

	/// Get the pointer to the node that is the closest to the given input position. If error return 0, else 1.
	int GetNodeClosestToPosition(mxPoint &input_position, mxSkeletonNode **output_closest_node, unsigned int t=0);

	/// Get the pointer to the link that is the closest to the given input position. If error return 0, else 1.
	int GetLinkClosestToPosition(mxPoint &input_position, mxSkeletonLink **output_closest_link, unsigned int t=0);

	/// Get the longest link adjacent to input node.
	mxSkeletonLink* GetLinkOfMaxLenghtAdjacentToNode(mxSkeletonNode *node);
	
	/// Finds the minimum and maximum value for scalars with given index in the links of the sekelton (nodes not taken into account).
	int GetScalarMinAndMaxValue(unsigned int index, double &output_min, double &output_max, unsigned int t=0);

	/// Calculates the length of the input skeleton link (spacing of skeleton is taken into account). If error, returns 0.
	/// The length is calculated from the start node to the end node.
	int GetLinkLength(mxSkeletonLink *p_input_skeleton_link, double *distance);
	double GetLinkLength(mxSkeletonLink *p_input_skeleton_link);//Unsecure version.

	/// Calculates the total length of skeleton by summing up the lengths of all links.
	double LengthOfSkeleton(unsigned int t=0);

	/// Finds all the stub nodes and puts their addresses to the list. Success 1, fail 0.
	int GetListOfStubNodes(mxList<mxSkeletonNode*> &list_of_stub_node_pointers, unsigned int t=0);

	/// Finds all the stub nodes and free nodes (without any links) and puts their addresses to the list. Success 1, fail 0.
	int GetListOfStubAndFreeNodes(mxList<mxSkeletonNode*> &list_of_stub_and_free_node_pointers, unsigned int t=0);

	/// For existing node and link positions creates skeletonization voxels (any existing voxels will be deleted) 
	/// by connecting given positions with lines. Scalar index determines which scalars are recorded as voxel values.
	int CreateSkeletonizationVoxelsFromPositions(unsigned int scalar_index = 0, unsigned int t=0);

	/// Makes a correction in the links of the skeleton, so that there are no multiple positions entered in the link and 
	/// that no entered link positions are the same as the start_node or end_node position.
	void CorrectLinks();

	/// Add new node to the skeleton with the position 'node_position'. First checks if it is already in skeleton. If the node with that position is 
	/// in skeleton, returns 0, but output node pointer WILL HAVE the address of the existing node. If not, adds the node with returning 1 and pointer 
	/// to it. After this, the node will have no links. THIS FUNCTION IS FOR 'MANUAL' BUILDING OF SKELETON. USE WITH CARE !!!
	int AddNodeWithPosition(mxPoint &node_position, mxSkeletonNode **output_added_node, unsigned int t=0);

	/// Add the link between nodes whose pointers are given. Link is added to the skeleton which has pnode1 as the 
	/// start node pointer (and its position) and pnode2 as the end node pointer (and its position). The list of positions of
	/// the link will be empty! Pointer to added link will be in output_link_pointer. Also adds pointer to link into both nodes
	/// Function does not check correctness of pnode1 and pnode2 (assumes they are pointing to skeleton nodes).
	/// THIS FUNCTION IS FOR 'MANUAL' BUILDING OF SKELETON. USE WITH CARE !!!
	void AddLinkBetweenTwoExistingNodes(mxSkeletonNode *pnode1, mxSkeletonNode *pnode2, mxSkeletonLink **output_link_pointer);

	/// Delete the skeleton link with pointer psl. Remove from list of links in skeleton and modifies list of pointers to links in nodes.
	/// If a node had only one link before the link was deleted (which means that it is left with no links), the node is also deleted.
	int RemoveLink(mxSkeletonLink *psl);

	/// Delete the skeleton link with pointer psl. Remove from list of links in skeleton and modifies list of pointers to links in nodes.
	/// If a node had only one link before the link was deleted (which means that it is left with no links), the node is also deleted.
	/// If any of the nodes after deletind of links has 2 links, do merging of node with its links using MergeNodeWithLinks() function.
	int RemoveLinkWithMerging(mxSkeletonLink *psl);

	/// Removes the node and all the links connected to the node from the skeleton.
	int RemoveNode(mxSkeletonNode *psn);

	/// Removes the input skeleton from this skeleton by checking all the links that are the same. Returns 1 if success, 0 fail.
	int RemoveSkeletonFromThis(mxSkeleton &skeleton_to_be_removed, mxSkeleton &output_skeleton, unsigned int t1=0, unsigned int t2=0);

	/// If the node has ONLY 2 links, which means that it is part of the path, merge the node into the path of its links. The result 
	/// will be a single link made out of the two links, without the node in between (the node will be deleted).
	int MergeNodeWithLinksForGivenNode(mxSkeletonNode *pnode);

	/// Performs MergeNodeWithLinksForGivenNode() on each skeleton node.
	void MergeNodeWithLinksForAllSkeletonNodes();
	void MergeNodeWithLinksForAllSkeletonNodes(unsigned int t);

	/// This method will merge 2 nodes so that psn2 will go into psn1_priority (so the psn1_priority is modified, but stays in the skeleton).
	/// The link psl will go into psn1_priority, while all other links connecting these 2 nodes will become loops.
	int MergeTwoAdjacentNodesConnectedByGivenLink(mxSkeletonNode *psn1_priority, mxSkeletonNode *psn2, mxSkeletonLink *psl);

	/// Remove stub branches that have the number of positions in link less or equal to 'threshold_number_of_positions_in_link'.
	/// Stub branch is a link with at least one node that has only one link connected to it. Deletes link and the node. 
	int FilterStubBranches(unsigned int threshold_number_of_positions_in_link);
	int FilterStubBranches(unsigned int threshold_number_of_positions_in_link, unsigned int t);

	/// Remove the links that are "tied in circle" (links that have the same node as the start and end node).
	int FilterLoops();
	int FilterLoops(unsigned int t);

	/// Finds links that all have the same start nodes and the same end nodes (links connect the same two nodes), which have the number of
	/// positions less or equal to 'threshold_number_of_positions_in_link'. From these links a single link is produced that has number of
	/// positions equal to max number of positions in all the links that are filtered. CAUTION: IT DOES NOT FILTER LOOPS!
	/// RETURNS 1 IF FILTERING WAS DONE, 0 IF NODES DID NOT HAVE MULTIPLE PATHS BETWEEN THEM!
	int FilterMultiplePathsForGivenNodes(mxSkeletonNode *pnode1, mxSkeletonNode *pnode2, int threshold_number_of_positions_in_link=10);

	/// Finds links that all have the same start nodes and the same end nodes (links connect the same two nodes), which have the number of
	/// positions less or equal to 'threshold_number_of_positions_in_link'. From these links a single link is produced that has number of
	/// positions equal to max number of positions in all the links that are filtered. CAUTION: IT DOES NOT FILTER LOOPS!
	int FilterMultiplePaths(int threshold_number_of_positions_in_link, unsigned int t);//NO DEFAULT VALUE HERE!
	int FilterMultiplePaths(int threshold_number_of_positions_in_link=10);

	/// Paste positions connected with lines in the skeleton link to an image (no scalars used). All pixels will be added with value 255.
	/// If 'set_output_size_automatically' is set, the output image size is determined in the method, if not set, the output image size 
	/// MUST be initialized before calling this function!
	int PasteTo3DImage(mxImage &output_binary_skeleton_image, unsigned int scalar_index=0, int set_output_size_automatically=1, unsigned int t=0);

	/// This function smooths all link paths with the input coefficients. For current position P[i] the new position is:		
	/// P[i]_new = ( P[i-3]*prev_3 + P[i-2]*prev_2 + P[i-1]*prev_1 + P[i] + P[i+1]*next_1 + P[i+2]*next_2 + P[i+3]*next_3 ) / n;
	/// where n is the sum of all the coefficients+1 for the coefficients involved in calculation (neighboring positions taken into account).
	/// Node positions are the only ones not affected by smoothing. If any of the previous(next) 3 positions is the node position,
	/// its effect in smoothing is calculated as e.g.: prev_x*P[i-x]*node_point_strength, where P[i-x] is node position. This insures 
	/// convergence of the smoothed path towards the nodes depending on the node_point_strength coefficient. If you want no special convergence
	/// set it to: node_point_strength = 1. 
	void Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength, unsigned int t);
	void Smooth(double prev_3, double prev_2, double prev_1, double next_1, double next_2, double next_3, double node_point_strength);

	/// Copy the selected link from this skeleton to the output skeleton (if the link does not already exist in the output skeleton - this check is done!). 
	/// Start and end node of the link will also be copied. If the output skeleton is not empty, the link will be added to existing skeleton structure. 
	/// If the output skeleton does not exist (is empty), we set the spacing and origin to it from this skeleton.
	/// CAUTION!!! IF THE SPACING AND ORIGIN OF OUTPUT AND THIS SKELETON ARE NOT THE SAME, THEY ARE NOT TAKEN INTO ACCOUNT!!!!
	int CopyLinkWithItsNodesToOutputSkeleton(mxSkeletonLink *psl, mxSkeleton *p_output_skeleton, unsigned int t2=0);

	/// Copy the selected link. Adds the nodes, where the positions of nodes are first and last positions in link positions. This copying method is needed
	/// when the skeleton is modified and the original configuration needs to be restoreed in a new skeleton. In order to store the positions of nodes
	/// into the links, the StoreNodePositionsToLinks() method is called. 
	int CopyLinkWithNodesAsFinalPositionsInLink(mxSkeletonLink *psl, mxSkeleton *p_output_skeleton, unsigned int t2=0);

	/// Full copying of the skeleton - use this function to make an exact copy!
	int CopyFrom(mxSkeleton *p_input_skeleton);
	int CopyFrom(mxSkeleton &input_skeleton);
    virtual int Copy(mxSkeleton *p_input_skeleton) { return this->CopyFrom(p_input_skeleton); };
    
    /// Copy data members of input object if the input object and this object are of the same type.
    virtual int CopyFromDataObject(mxDataObject *data_object);
    
    /// Create a skeleton from input point list.
    int CreateFromPointList(mxPointList *pl);

    /// Copy the input skeleton to this skeleton (appends structure).
    int Add(mxSkeleton &input_skeleton);
    int Add(mxSkeleton *input_skeleton);
    
	/// For each link adds the start node position to left of position list and end node position to right end of position list. This way the original 
	/// node positions are preserved if the merging/shrinking of the graph is done. BEWARE TO EXTRACT THE ADDED NODE POSITIONS TO GET ORIGINAL LINK POSITIONS!!!
	int StoreNodePositionsToLinks();
	int StoreNodePositionsToLinks(unsigned int t);

	/// Grows a skeleton from 'p_node' as a starting point(node) and does it in 'number_of_steps' growing steps. The 'p_limiting_skeleton' must be a skeleton
	/// containing the structure which is a part of this skeleton and it is used to limit the growth of new skeleton - the growth does not go into the structure
	/// belonging to 'p_limiting_skeleton'. By default, there is no limiting skeleton (so pointer to it is NULL). The node 'p_node' MUST be a part of this
	/// skeleton. The output indicator 'is_skeleton_closed' gives a return value which indicated that the output skeleton is closed (has no external links) for 
	/// the given number of growing steps. If output skeleton is closed 'is_skeleton_closed' is 1, if open 'is_skeleton_closed' is 0. Return: success 1, fail 0.
	int GrowSkeletonFromNode(mxSkeletonNode *p_node, int number_of_steps, mxSkeleton *p_output_skeleton, int *is_skeleton_closed, mxSkeleton *p_limiting_skeleton=NULL, unsigned int t2=0);

	/// Grow a skeleton from this skeleton and a given start node (in this skeleton). Use this method when extracting a connected part of disconnected skeleton.
	int GrowSkeletonFromNode(mxSkeletonNode *p_node, mxSkeleton &output_sk, unsigned int t2=0);

	/// Calculates the 3D radius at each position of the skeleton using the segmented image and store to input tag scalars.
	/// Radius is the maximum inscribed sphere from skeleton position to vessel border. Succes 1, fail 0.
	int RadiusAtEachPosition(mxImage &segmented_image, mxString &scalar_tag, unsigned int t=0);

	/// Store node pointers into array- in this way we can access a node in the skeleton and using the index in the array.
	int GetArrayOfSkeletonNodes(mxArray<mxSkeletonNode*> &array_of_node_pointers, unsigned int t=0);

	/// For 2 exactly the same skeletons, store node pointers into arrays- in this way we can access a node in the first skeleton and using the same index in the array
	/// promptly access the corresponding node in the second skeleton (through the scond array). CAUTION! SKELETONS MUST BE EXACTLY THE SAME!
	int GetArraysOfCorrespondingSkeletonNodesInThisAndInputSkeleton(mxSkeleton &input_sk, mxArray<mxSkeletonNode*> &array_of_node_addresses1, mxArray<mxSkeletonNode*> &array_of_node_addresses2, unsigned int t1=0, unsigned int t2=0);

	/// For 2 exactly the same skeletons, store link pointers into arrays- in this way we can access a link in the first skeleton and using the same index in the array
	/// promptly access the corresponding link in the second skeleton (through the scond array). CAUTION! SKELETONS MUST BE EXACTLY THE SAME!
	int GetArraysOfCorrespondingSkeletonLinksInThisAndInputSkeleton(mxSkeleton &input_sk, mxArray<mxSkeletonLink*> &array_of_node_addresses1, mxArray<mxSkeletonLink*> &array_of_node_addresses2, unsigned int t1=0, unsigned int t2=0);

/// ===== METHODS FOR SETTING LINK METRICS AND FINDING BEST PATHS =====
/// Set the METRIC of the link. This is used for best path calculation. 
/// Must be called prior to SetConvergencePoint_Propagate___Value() function.

	/// Sets the link metrics according to the order in which the stub links would be removed. Use this before calling ThresholdByLinkMetricsToLeaveGivenNumberOfStubLinks()
	/// The ORDER IS BASED ON LINK LENGTH (stub links will have different metrics based on their lengths).
	int SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging();
	int SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging(unsigned int t);

	/// Sets the link metrics according to the order in which the stub links would be removed. Use this before calling ThresholdByLinkMetricsToLeaveGivenNumberOfStubLinks()
	/// The ORDER IS EQUAL TO STUB LINK REMOVAL STEP (all stub links removed in the same step will have the same metrics
	/// (metric will be equal to the number of the step in which they are removed).
	int SetLinkMetricsAsStubLinkRemovalStepNumber();
	int SetLinkMetricsAsStubLinkRemovalStepNumber(unsigned int t);

	/// Sets the link values corresponding to input tag from link metrics. Return success 1, fail 0.
	int SetLinkValuesFromLinkMetrics(mxString &tag);
	int SetLinkValuesFromLinkMetrics(mxString &tag, unsigned int t);

	/// Prune the skeleton links by their metrics until the resulting skeleton has at least 'number_of_stub_links' stub links. It can happen that the resulting 
	/// skeleton has more stub links than specified. CAUTION: CALL SetLinkMetricsAsOrderOfStubLinkFilteringAndNodeMerging() BEFORE THIS METHOD!
	int PruningByLinkMetricsToLeaveGivenNumberOfStubLinks(mxSkeleton &output_sk, unsigned int number_of_stub_links, unsigned int t, unsigned int t2);
	int PruningByLinkMetricsToLeaveGivenNumberOfStubLinks(mxSkeleton &output_sk, unsigned int number_of_stub_links);

	/// Set the METRIC of the link to be equal to MINIMUM value of link scalars with given index.
	int SetLinkMetricsToMinOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToMinOfScalars(unsigned int scalar_index, unsigned int t);

	/// Set the METRIC of the link to be equal to MAXIMUM value of link scalars with given index.
	int SetLinkMetricsToMaxOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToMaxOfScalars(unsigned int scalar_index, unsigned int t);

	/// Set the METRIC of the link to be equal to AVERAGE value of link scalars with given index.
	int SetLinkMetricsToAverageOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToAverageOfScalars(unsigned int scalar_index, unsigned int t);

	/// Set the METRIC of the link to be equal to MINIMUM value of link scalars with given index.
	int SetLinkMetricsToInverseOfMinOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToInverseOfMinOfScalars(unsigned int scalar_index, unsigned int t);

	/// Set the METRIC of the link to be equal to MAXIMUM value of link scalars with given index.
	int SetLinkMetricsToInverseOfMaxOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToInverseOfMaxOfScalars(unsigned int scalar_index, unsigned int t);

	/// Set the METRIC of the link to be equal to AVERAGE value of link scalars with given index.
	int SetLinkMetricsToInverseOfAverageOfScalars(unsigned int scalar_index);
	int SetLinkMetricsToInverseOfAverageOfScalars(unsigned int scalar_index, unsigned int t);


	/// Set the METRIC of the link to average of the number of links the nodes (that this link connects) have.
	int SetLinkMetricsToAverageNumberOfLinksOfConnectingNodes();
	int SetLinkMetricsToAverageNumberOfLinksOfConnectingNodes(unsigned int t);

	/// Set the METRIC of the link to be equal to the link length.
	int SetLinkMetricsToLinkLength();
	int SetLinkMetricsToLinkLength(unsigned int t);

	/// Set the METRIC of the link to input value. 
	int SetLinkMetricsTo(double metric_value);
	int SetLinkMetricsTo(double metric_value, unsigned int t);

	/// Set the METRICs of links to number of steps from input (root) link. THIS METHOD WORKS ONLY ON 3D PART TO WHICH THE INPUT LINK BELONGS!
	int SetLinkMetricsToHierarchyFromRoot(mxSkeletonLink *root_link);

	/// Set the METRICs of links to a unique number (label).
	int SetLinkMetricsToSequentialNumbering();
	int SetLinkMetricsToSequentialNumbering(unsigned int t);

	/// For every position in links and nodes assign a unique number (label) value with given tag.
	int SetLinkAndNodeScalarValuesToPositionSequentialNumbering(mxString &tag);
	int SetLinkAndNodeScalarValuesToPositionSequentialNumbering(mxString &tag, unsigned int t);

	/// Converge the skeleton as graph from given start node. Needed before the best path calculation. This node should be the one that is not often changed.
	/// Priority is given to GREATER (HIGHER) link values. !!! NOTICE: LINK METRICS HAVE TO BE SET BEFORE CALLING THIS FUNCTION (call SetLinkMetrics...() method).
	/// THIS METHOD WORKS ONLY ON 3D PART TO WHICH THE INPUT LINK BELONGS!
	void SetConvergencePoint_PropagateHigherValue(mxSkeletonNode *input_node);

	/// Converge the skeleton as graph from given start node. Needed before the best path calculation. This node should be the one that is not often changed.
	/// Priority is given to LOWER (SMALLER) link values. !!! NOTICE: LINK METRICS HAVE TO BE SET BEFORE CALLING THIS FUNCTION (call SetLinkMetrics...() method).
	void SetConvergencePoint_PropagateLowerValue(mxSkeletonNode *input_node);

	/// Converge the skeleton as graph from given start node. Needed before the best path calculation. This node should be the one that is not often changed.
	/// Priority is given to GREATER (HIGHER) link values. !!! NOTICE: LINK METRICS HAVE TO BE SET BEFORE CALLING THIS FUNCTION (call SetLinkMetrics...() method).
	/// THIS ONE WORKS WITH 'metric_value_2' IN NODES (SO IT IS USED TO FIND CONVERGENCE TO AN ALTERNATIVE NODE).
	void SetConvergencePoint_PropagateHigherValue_2(mxSkeletonNode *input_node);

	/// Converge the skeleton as graph from given start node. Needed before the best path calculation. This node should be the one that is not often changed.
	/// Priority is given to LOWER (SMALLER) link values. !!! NOTICE: LINK METRICS HAVE TO BE SET BEFORE CALLING THIS FUNCTION (call SetLinkMetrics...() method).
	/// THIS ONE WORKS WITH 'metric_value_2' IN NODES (SO IT IS USED TO FIND CONVERGENCE TO AN ALTERNATIVE NODE).
	void SetConvergencePoint_PropagateLowerValue_2(mxSkeletonNode *input_node);

	/// Gets the best path from 'destination_node' to source node set by SetConvergencePoint_Propagate...() method.
	int BestPathToNode(mxSkeletonNode *destination_node, mxSkeleton *p_output_skeleton, unsigned int t2=0);

	/// Get the best path from 'destination_node' to source node set by SetConvergencePoint_Propagate..._2() method. WORKS WITH 'metric_value_2' IN NODES.
	int BestPathToNode_2(mxSkeletonNode *destination_node, mxSkeleton *p_output_skeleton, unsigned int t2=0);

	/// Get the best path from the given node to two source nodes set by SetConvergencePoint_Propagate...() and SetConvergencePoint_Propagate..._2() methods.
	/// Therefore, the output skeleton connects two source nodes over current_node where the calculated distance is in 'distance'.
	int BestPathToTwoConvergedNodes(mxSkeletonNode *current_node, mxSkeleton *p_output_skeleton, double *distance, unsigned int t2=0);



//    /// Saving Skeleton to SKL file. After "l" links positions are recorded WITH node positions followed by
//    /// link skeletonization voxels. After "n" node positions are recorded followed by node skeletonization' voxels.
//    /// CAUTION: loads max 100000 bytes text line (but no file size limit), anything beyond that is discarded.
//    void SaveToTextFileOfPositions_v1(const char *file_name);
//
//
//    /// This version saves time instances (4D skeleton)
//    void SaveToTextFileOfPositions_v2(const char *file_name, const char *comment=NULL);

    /// Save to 'skl' file (text).
    void SaveToTextFileOfPositions_v3(const char *file_name, const char *comment=NULL);

    
	/// Get the main (integer) version number of the skeleton (.skl) file. This method is used to determine with which loading method to proceed.
	int FileVersion(const char *file_name, int &output_version);

	/// This version supports only 3D skeletons.
    int LoadFromTextFileOfPositions_v1(const char *file_name);	//int LoadFromTextFileOfPositions_v1(char *file_name_root, char *extension);

	/// Method for loading 4D skeleton.
	int LoadFromTextFileOfPositions_v2(const char *file_name, mxString *output_comment=NULL);
    int LoadFromTextFileOfPositions_v3(const char *file_name, mxString *output_comment=NULL);
    
    /// Legacy metho for loading skeleton.
    int LoadFromTextFileOfPositions_Legacy(const char *file_name);


	/// General saving/loading methods.
	void SaveToTextFileOfPositions(const char *file_name);
	int LoadFromTextFileOfPositions(const char *file_name);

    
    /// Save to a default file type.
    void SaveToDefaultFileType(const char *file_path);

    /// Creates a PATH (skeleton with one link) from given WORLD positions. String should be of type:
    /// |t1, s1, r1, c1||t2, s2, r2, c2||...||tn, sn, rn, cn|
    int ReadPathFromStringOfPositions(const char *string_of_world_positions);



};

#endif
