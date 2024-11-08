// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2020 - 2024 Intel Corporation

#include <linux/err.h>
#include <linux/string.h>

#include "ipu-psys.h"
#include "ipu-fw-psys.h"
#include "ipu6-platform-resources.h"
#include "ipu6ep-platform-resources.h"

/* resources table */

/*
 * Cell types by cell IDs
 */
static const u8 ipu6ep_fw_psys_cell_types[IPU6EP_FW_PSYS_N_CELL_ID] = {
	IPU6_FW_PSYS_SP_CTRL_TYPE_ID,
	IPU6_FW_PSYS_VP_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_OSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_OSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_OSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_PSA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID,
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID, /* AF */
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID, /* X2B_MD */
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID, /* X2B_SVE_RGBIR */
	IPU6_FW_PSYS_ACC_ISA_TYPE_ID, /* PAF */
	IPU6_FW_PSYS_GDC_TYPE_ID,
	IPU6_FW_PSYS_TNR_TYPE_ID,
};

static const u16 ipu6ep_fw_num_dev_channels[IPU6_FW_PSYS_N_DEV_CHN_ID] = {
	IPU6_FW_PSYS_DEV_CHN_DMA_EXT0_MAX_SIZE,
	IPU6_FW_PSYS_DEV_CHN_DMA_EXT1_READ_MAX_SIZE,
	IPU6_FW_PSYS_DEV_CHN_DMA_EXT1_WRITE_MAX_SIZE,
	IPU6_FW_PSYS_DEV_CHN_DMA_INTERNAL_MAX_SIZE,
	IPU6_FW_PSYS_DEV_CHN_DMA_ISA_MAX_SIZE,
};

static const u16 ipu6ep_fw_psys_mem_size[IPU6_FW_PSYS_N_MEM_ID] = {
	IPU6_FW_PSYS_VMEM0_MAX_SIZE,
	IPU6_FW_PSYS_TRANSFER_VMEM0_MAX_SIZE,
	IPU6_FW_PSYS_TRANSFER_VMEM1_MAX_SIZE,
	IPU6_FW_PSYS_LB_VMEM_MAX_SIZE,
	IPU6_FW_PSYS_BAMEM0_MAX_SIZE,
	IPU6_FW_PSYS_DMEM0_MAX_SIZE,
	IPU6_FW_PSYS_DMEM1_MAX_SIZE,
	IPU6_FW_PSYS_DMEM2_MAX_SIZE,
	IPU6_FW_PSYS_DMEM3_MAX_SIZE,
	IPU6_FW_PSYS_PMEM0_MAX_SIZE
};

static const u16 ipu6ep_fw_psys_dfms[IPU6_FW_PSYS_N_DEV_DFM_ID] = {
	IPU6_FW_PSYS_DEV_DFM_BB_FULL_PORT_ID_MAX_SIZE,
	IPU6_FW_PSYS_DEV_DFM_BB_EMPTY_PORT_ID_MAX_SIZE,
	IPU6_FW_PSYS_DEV_DFM_ISL_FULL_PORT_ID_MAX_SIZE,
	IPU6_FW_PSYS_DEV_DFM_ISL_EMPTY_PORT_ID_MAX_SIZE,
	IPU6_FW_PSYS_DEV_DFM_LB_FULL_PORT_ID_MAX_SIZE,
	IPU6_FW_PSYS_DEV_DFM_LB_EMPTY_PORT_ID_MAX_SIZE,
};

static const u8
ipu6ep_fw_psys_c_mem[IPU6EP_FW_PSYS_N_CELL_ID][IPU6_FW_PSYS_N_MEM_TYPE_ID] = {
	{
		/* IPU6_FW_PSYS_SP0_ID */
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_DMEM0_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_SP1_ID */
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_DMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_VP0_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_DMEM3_ID,
		IPU6_FW_PSYS_VMEM0_ID,
		IPU6_FW_PSYS_BAMEM0_ID,
		IPU6_FW_PSYS_PMEM0_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC1_ID BNLM */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC2_ID DM */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC3_ID ACM */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC4_ID GTC YUV1 */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC5_ID OFS pin main */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC6_ID OFS pin display */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC7_ID OFS pin pp */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC8_ID GAMMASTAR */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC9_ID GLTM */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ACC10_ID XNR */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_ICA_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_LSC_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_DPC_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_SIS_A_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_SIS_B_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_B2B_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_B2R_ID and ISA_R2I_SIE */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_R2I_DS_A_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_AWB_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_AE_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_AF_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_X2B_MD_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_X2B_SVE_RGBIR_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_ISA_PAF_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_LB_VMEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_BB_ACC_GDC0_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	},
	{
		/* IPU6_FW_PSYS_BB_ACC_TNR_ID */
		IPU6_FW_PSYS_TRANSFER_VMEM0_ID,
		IPU6_FW_PSYS_TRANSFER_VMEM1_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
		IPU6_FW_PSYS_N_MEM_ID,
	}
};

static const struct ipu_fw_resource_definitions ipu6ep_defs = {
	.cells = ipu6ep_fw_psys_cell_types,
	.num_cells = IPU6EP_FW_PSYS_N_CELL_ID,
	.num_cells_type = IPU6_FW_PSYS_N_CELL_TYPE_ID,

	.dev_channels = ipu6ep_fw_num_dev_channels,
	.num_dev_channels = IPU6_FW_PSYS_N_DEV_CHN_ID,

	.num_ext_mem_types = IPU6_FW_PSYS_N_DATA_MEM_TYPE_ID,
	.num_ext_mem_ids = IPU6_FW_PSYS_N_MEM_ID,
	.ext_mem_ids = ipu6ep_fw_psys_mem_size,

	.num_dfm_ids = IPU6_FW_PSYS_N_DEV_DFM_ID,

	.dfms = ipu6ep_fw_psys_dfms,

	.cell_mem_row = IPU6_FW_PSYS_N_MEM_TYPE_ID,
	.cell_mem = &ipu6ep_fw_psys_c_mem[0][0],
};

const struct ipu_fw_resource_definitions *ipu6ep_res_defs = &ipu6ep_defs;