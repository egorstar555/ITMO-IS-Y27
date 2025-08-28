package com.Starygin.view;

import com.Starygin.model.entity.Owner;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.OwnerService;
import com.Starygin.service.exceptions.OwnerIdMismatchException;
import com.querydsl.core.types.Predicate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Pageable;
import org.springframework.data.querydsl.binding.QuerydslPredicate;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;

@RestController
@RequestMapping("/api/owners")
public class OwnerController {

    private final OwnerService ownerService;

    @Autowired
    public OwnerController(OwnerService ownerService) {
        this.ownerService = ownerService;
    }

    @GetMapping
    public List<OwnerDto> findAll() {
        return ownerService.findAll();
    }

    @GetMapping("/page-and-filter")
    public List<OwnerDto> findAllByPageAndFilter(
            @QuerydslPredicate(root = Owner.class) Predicate predicate,
            Pageable pageable) {

        return ownerService.findAllByPageAndFilter(predicate, pageable);
    }

    @GetMapping("/name/{ownerName}")
    public List<OwnerDto> findByName(@PathVariable String ownerName) {
        return ownerService.findByName(ownerName);
    }

    @GetMapping("/{id}")
    public OwnerDto findOne(@PathVariable Long id) {
        return ownerService.findOne(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public OwnerDto create(@RequestBody OwnerDto ownerDto) {
        return ownerService.create(ownerDto);
    }

    @DeleteMapping("/{id}")
    public void delete(@PathVariable Long id) {
        ownerService.delete(id);
    }

    @PutMapping("/{id}")
    public OwnerDto updateOwner(@RequestBody OwnerDto ownerDto, @PathVariable Long id) {
        if (!Objects.equals(ownerDto.id(), id)) {
            throw new OwnerIdMismatchException();
        }

        return ownerService.updateOwner(ownerDto, id);
    }
}
