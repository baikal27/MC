<distribution version="8.5.0" name="Tcs_ajin" type="MSI">
	<msi GUID="{B480664B-8E4F-4872-9ABB-45FC35BA8EEF}">
		<general appName="Tcs_ajin" outputLocation="d:\tcs_ajin_new\cvidistkit.Tcs_ajin" relOutputLocation="cvidistkit.Tcs_ajin" outputLocationWithVars="d:\tcs_ajin_new\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" autoIncrement="true" version="1.0.3">
			<arp company="KASI" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" readMe="" license=""><dlgstrings welcomeTitle="Tcs_ajin" welcomeText="">
			</dlgstrings></userinterface>
		<dirs appDirID="100">
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true"/>
			<installDir name="Tcs_ajin" dirID="100" parentID="2" isMSIDir="false" visible="true"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true"/>
			<installDir name="Tcs_ajin" dirID="101" parentID="7" isMSIDir="false" visible="true"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="D:\tcs_ajin_new\AXLNet.dll" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="d:\tcs_ajin_new\tcs.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="d:\tcs_ajin_new\catalog.uir" relSourcePath="catalog.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="d:\tcs_ajin_new\ec_test.uir" relSourcePath="ec_test.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="d:\tcs_ajin_new\guide_sub.uir" relSourcePath="guide_sub.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="d:\tcs_ajin_new\sky_scan.uir" relSourcePath="sky_scan.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="d:\tcs_ajin_new\TCS.uir" relSourcePath="TCS.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="d:\tcs_ajin_new\tcs_ajin.uir" relSourcePath="tcs_ajin.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>1</fileID></projectOutput>
			<projectDependencies dirID="100" projectID="0">
				<fileID>0</fileID></projectDependencies></fileGroups>
		<shortcuts>
			<shortcut name="Tcs_ajin" targetFileID="1" destDirID="101" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine cvirte="true" instrsup="true" lvrt="true" analysis="true" netvarsup="true" dotnetsup="true" activeXsup="true" lowlevelsup="true" rtutilsup="true" installToAppDir="false"/>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WIN2K_SP3</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="d:\tcs_ajin_new\tcs_ajin.prj" ProjectRelativePath="tcs_ajin.prj"/></Projects>
		<buildData progressBarRate="0.035267148651032">
			<progressTimes>
					<Begin>0.000000000000000</Begin>
					<ProductsAdded>0.000000000000000</ProductsAdded>
					<DPConfigured>1.297000408172607</DPConfigured>
					<DPMergeModulesAdded>2.063000202178955</DPMergeModulesAdded>
					<DPClosed>4.781000137329102</DPClosed>
					<DistributionsCopied>4.797000408172607</DistributionsCopied>
					<End>28.355000000000000</End></progressTimes></buildData>
	</msi>
</distribution>
