
a.out:     file format elf32-i386

Disassembly of section .init:

0804822c <_init>:
 804822c:	55                   	push   %ebp
 804822d:	89 e5                	mov    %esp,%ebp
 804822f:	83 ec 08             	sub    $0x8,%esp
 8048232:	e8 51 00 00 00       	call   8048288 <call_gmon_start>
 8048237:	e8 ac 00 00 00       	call   80482e8 <frame_dummy>
 804823c:	e8 7f 01 00 00       	call   80483c0 <__do_global_ctors_aux>
 8048241:	c9                   	leave  
 8048242:	c3                   	ret    
Disassembly of section .plt:

08048244 <.plt>:
 8048244:	ff 35 f8 94 04 08    	pushl  0x80494f8
 804824a:	ff 25 fc 94 04 08    	jmp    *0x80494fc
 8048250:	00 00                	add    %al,(%eax)
 8048252:	00 00                	add    %al,(%eax)
 8048254:	ff 25 00 95 04 08    	jmp    *0x8049500
 804825a:	68 00 00 00 00       	push   $0x0
 804825f:	e9 e0 ff ff ff       	jmp    8048244 <_init+0x18>
Disassembly of section .text:

08048264 <_start>:
 8048264:	31 ed                	xor    %ebp,%ebp
 8048266:	5e                   	pop    %esi
 8048267:	89 e1                	mov    %esp,%ecx
 8048269:	83 e4 f0             	and    $0xfffffff0,%esp
 804826c:	50                   	push   %eax
 804826d:	54                   	push   %esp
 804826e:	52                   	push   %edx
 804826f:	68 7c 83 04 08       	push   $0x804837c
 8048274:	68 34 83 04 08       	push   $0x8048334
 8048279:	51                   	push   %ecx
 804827a:	56                   	push   %esi
 804827b:	68 10 83 04 08       	push   $0x8048310
 8048280:	e8 cf ff ff ff       	call   8048254 <_init+0x28>
 8048285:	f4                   	hlt    
 8048286:	90                   	nop    
 8048287:	90                   	nop    

08048288 <call_gmon_start>:
 8048288:	55                   	push   %ebp
 8048289:	89 e5                	mov    %esp,%ebp
 804828b:	53                   	push   %ebx
 804828c:	e8 00 00 00 00       	call   8048291 <call_gmon_start+0x9>
 8048291:	5b                   	pop    %ebx
 8048292:	81 c3 63 12 00 00    	add    $0x1263,%ebx
 8048298:	50                   	push   %eax
 8048299:	8b 83 10 00 00 00    	mov    0x10(%ebx),%eax
 804829f:	85 c0                	test   %eax,%eax
 80482a1:	74 02                	je     80482a5 <call_gmon_start+0x1d>
 80482a3:	ff d0                	call   *%eax
 80482a5:	8b 5d fc             	mov    0xfffffffc(%ebp),%ebx
 80482a8:	c9                   	leave  
 80482a9:	c3                   	ret    
 80482aa:	90                   	nop    
 80482ab:	90                   	nop    

080482ac <__do_global_dtors_aux>:
 80482ac:	55                   	push   %ebp
 80482ad:	89 e5                	mov    %esp,%ebp
 80482af:	83 ec 08             	sub    $0x8,%esp
 80482b2:	80 3d 08 95 04 08 00 	cmpb   $0x0,0x8049508
 80482b9:	75 29                	jne    80482e4 <__do_global_dtors_aux+0x38>
 80482bb:	a1 14 94 04 08       	mov    0x8049414,%eax
 80482c0:	8b 10                	mov    (%eax),%edx
 80482c2:	85 d2                	test   %edx,%edx
 80482c4:	74 17                	je     80482dd <__do_global_dtors_aux+0x31>
 80482c6:	89 f6                	mov    %esi,%esi
 80482c8:	83 c0 04             	add    $0x4,%eax
 80482cb:	a3 14 94 04 08       	mov    %eax,0x8049414
 80482d0:	ff d2                	call   *%edx
 80482d2:	a1 14 94 04 08       	mov    0x8049414,%eax
 80482d7:	8b 10                	mov    (%eax),%edx
 80482d9:	85 d2                	test   %edx,%edx
 80482db:	75 eb                	jne    80482c8 <__do_global_dtors_aux+0x1c>
 80482dd:	c6 05 08 95 04 08 01 	movb   $0x1,0x8049508
 80482e4:	c9                   	leave  
 80482e5:	c3                   	ret    
 80482e6:	89 f6                	mov    %esi,%esi

080482e8 <frame_dummy>:
 80482e8:	55                   	push   %ebp
 80482e9:	89 e5                	mov    %esp,%ebp
 80482eb:	83 ec 08             	sub    $0x8,%esp
 80482ee:	a1 f0 94 04 08       	mov    0x80494f0,%eax
 80482f3:	85 c0                	test   %eax,%eax
 80482f5:	74 16                	je     804830d <frame_dummy+0x25>
 80482f7:	b8 00 00 00 00       	mov    $0x0,%eax
 80482fc:	85 c0                	test   %eax,%eax
 80482fe:	74 0d                	je     804830d <frame_dummy+0x25>
 8048300:	83 ec 0c             	sub    $0xc,%esp
 8048303:	68 f0 94 04 08       	push   $0x80494f0
 8048308:	ff d0                	call   *%eax
 804830a:	83 c4 10             	add    $0x10,%esp
 804830d:	c9                   	leave  
 804830e:	c3                   	ret    
 804830f:	90                   	nop    

08048310 <main>:
 8048310:	55                   	push   %ebp
 8048311:	89 e5                	mov    %esp,%ebp
 8048313:	83 ec 08             	sub    $0x8,%esp
 8048316:	83 e4 f0             	and    $0xfffffff0,%esp
 8048319:	b8 00 00 00 00       	mov    $0x0,%eax
 804831e:	29 c4                	sub    %eax,%esp
 8048320:	8b 45 fc             	mov    0xfffffffc(%ebp),%eax
 8048323:	c1 e8 1f             	shr    $0x1f,%eax
 8048326:	f7 d8                	neg    %eax
 8048328:	89 45 f8             	mov    %eax,0xfffffff8(%ebp)
 804832b:	b8 00 00 00 00       	mov    $0x0,%eax
 8048330:	c9                   	leave  
 8048331:	c3                   	ret    
 8048332:	90                   	nop    
 8048333:	90                   	nop    

08048334 <__libc_csu_init>:
 8048334:	55                   	push   %ebp
 8048335:	89 e5                	mov    %esp,%ebp
 8048337:	57                   	push   %edi
 8048338:	56                   	push   %esi
 8048339:	53                   	push   %ebx
 804833a:	83 ec 0c             	sub    $0xc,%esp
 804833d:	e8 00 00 00 00       	call   8048342 <__libc_csu_init+0xe>
 8048342:	5b                   	pop    %ebx
 8048343:	81 c3 b2 11 00 00    	add    $0x11b2,%ebx
 8048349:	e8 de fe ff ff       	call   804822c <_init>
 804834e:	8d 93 18 ff ff ff    	lea    0xffffff18(%ebx),%edx
 8048354:	8d 8b 18 ff ff ff    	lea    0xffffff18(%ebx),%ecx
 804835a:	29 ca                	sub    %ecx,%edx
 804835c:	c1 fa 02             	sar    $0x2,%edx
 804835f:	31 f6                	xor    %esi,%esi
 8048361:	39 d6                	cmp    %edx,%esi
 8048363:	73 0f                	jae    8048374 <__libc_csu_init+0x40>
 8048365:	89 d7                	mov    %edx,%edi
 8048367:	90                   	nop    
 8048368:	ff 94 b3 18 ff ff ff 	call   *0xffffff18(%ebx,%esi,4)
 804836f:	46                   	inc    %esi
 8048370:	39 fe                	cmp    %edi,%esi
 8048372:	72 f4                	jb     8048368 <__libc_csu_init+0x34>
 8048374:	83 c4 0c             	add    $0xc,%esp
 8048377:	5b                   	pop    %ebx
 8048378:	5e                   	pop    %esi
 8048379:	5f                   	pop    %edi
 804837a:	c9                   	leave  
 804837b:	c3                   	ret    

0804837c <__libc_csu_fini>:
 804837c:	55                   	push   %ebp
 804837d:	89 e5                	mov    %esp,%ebp
 804837f:	56                   	push   %esi
 8048380:	53                   	push   %ebx
 8048381:	e8 00 00 00 00       	call   8048386 <__libc_csu_fini+0xa>
 8048386:	5b                   	pop    %ebx
 8048387:	81 c3 6e 11 00 00    	add    $0x116e,%ebx
 804838d:	8d 8b 18 ff ff ff    	lea    0xffffff18(%ebx),%ecx
 8048393:	8d 83 18 ff ff ff    	lea    0xffffff18(%ebx),%eax
 8048399:	29 c1                	sub    %eax,%ecx
 804839b:	c1 f9 02             	sar    $0x2,%ecx
 804839e:	85 c9                	test   %ecx,%ecx
 80483a0:	8d 71 ff             	lea    0xffffffff(%ecx),%esi
 80483a3:	75 0b                	jne    80483b0 <__libc_csu_fini+0x34>
 80483a5:	e8 3a 00 00 00       	call   80483e4 <_fini>
 80483aa:	5b                   	pop    %ebx
 80483ab:	5e                   	pop    %esi
 80483ac:	c9                   	leave  
 80483ad:	c3                   	ret    
 80483ae:	89 f6                	mov    %esi,%esi
 80483b0:	ff 94 b3 18 ff ff ff 	call   *0xffffff18(%ebx,%esi,4)
 80483b7:	89 f2                	mov    %esi,%edx
 80483b9:	4e                   	dec    %esi
 80483ba:	85 d2                	test   %edx,%edx
 80483bc:	75 f2                	jne    80483b0 <__libc_csu_fini+0x34>
 80483be:	eb e5                	jmp    80483a5 <__libc_csu_fini+0x29>

080483c0 <__do_global_ctors_aux>:
 80483c0:	55                   	push   %ebp
 80483c1:	89 e5                	mov    %esp,%ebp
 80483c3:	53                   	push   %ebx
 80483c4:	52                   	push   %edx
 80483c5:	a1 e0 94 04 08       	mov    0x80494e0,%eax
 80483ca:	83 f8 ff             	cmp    $0xffffffff,%eax
 80483cd:	bb e0 94 04 08       	mov    $0x80494e0,%ebx
 80483d2:	74 0c                	je     80483e0 <__do_global_ctors_aux+0x20>
 80483d4:	83 eb 04             	sub    $0x4,%ebx
 80483d7:	ff d0                	call   *%eax
 80483d9:	8b 03                	mov    (%ebx),%eax
 80483db:	83 f8 ff             	cmp    $0xffffffff,%eax
 80483de:	75 f4                	jne    80483d4 <__do_global_ctors_aux+0x14>
 80483e0:	58                   	pop    %eax
 80483e1:	5b                   	pop    %ebx
 80483e2:	c9                   	leave  
 80483e3:	c3                   	ret    
Disassembly of section .fini:

080483e4 <_fini>:
 80483e4:	55                   	push   %ebp
 80483e5:	89 e5                	mov    %esp,%ebp
 80483e7:	53                   	push   %ebx
 80483e8:	e8 00 00 00 00       	call   80483ed <_fini+0x9>
 80483ed:	5b                   	pop    %ebx
 80483ee:	81 c3 07 11 00 00    	add    $0x1107,%ebx
 80483f4:	52                   	push   %edx
 80483f5:	e8 b2 fe ff ff       	call   80482ac <__do_global_dtors_aux>
 80483fa:	8b 5d fc             	mov    0xfffffffc(%ebp),%ebx
 80483fd:	c9                   	leave  
 80483fe:	c3                   	ret    
