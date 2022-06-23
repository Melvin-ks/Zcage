ZcageSystem（零笼-基于零知识证明协议与区块链技术的风险管理平台）是ZcageSystem开发小组开发的基于PLONK零知识证明协议和区块链技术的风险评估平台。
旨在将敏感数据禁锢在“零知识证明”所构筑的“笼”中。本平台适用于银行与中小微企业的融资借贷场景中，其应用场景得到了金融界从业人士的肯定。

本平台分为三个模块：
1.Zcage-Plonk：
	本模块是平台的核心算法部分，分为Plonk验证器以及Plonk证明器，核心算法完全自主实现，效率相比主流的snarkjs工具有很大提升。本项目的依赖库有：openssl,NTL,mcl,SM2-signature等。

2.Zcage-blockchain
	本模块是平台的区块链部分，基于微众银行的FISCO-BCOS金盟链进行开发，实现了链上存储证明，链上调用验证器功能，其中链上验证器使用C++编写，性能远超主流solidity实现。
3.Zcage-web
	本模块是平台网页部分，调用链上验证，回显验证结果，生成验证结果证书，屏蔽底层复杂性从而便于金融业从业人士轻松使用本平台功能。由于Proof整个生成过程均在证明者本地进行，我们提前在区块链上上传了多个公司的Proof，您可以在 Services->User->查询对象 键入companyA,companyB,companyC来查询相关公司的评估结果。




欢迎您贡献源码，祝您使用愉快！