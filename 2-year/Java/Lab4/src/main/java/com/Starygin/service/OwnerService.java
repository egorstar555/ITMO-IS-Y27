package com.Starygin.service;

import com.Starygin.model.repository.OwnerRepository;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.exceptions.OwnerIdMismatchException;
import com.Starygin.service.exceptions.OwnerNotFoundException;
import com.Starygin.service.mapper.OwnerMapper;
import com.querydsl.core.types.Predicate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Objects;

@Service
public class OwnerService {

    private final OwnerRepository ownerRepository;

    private final OwnerMapper ownerMapper;

    @Autowired
    public OwnerService(OwnerRepository ownerRepository, OwnerMapper ownerMapper) {
        this.ownerRepository = ownerRepository;
        this.ownerMapper = ownerMapper;
    }

    public List<OwnerDto> findAll() {
        return ownerRepository.findAll().stream().map(ownerMapper::toDto).toList();
    }

    public List<OwnerDto> findAllByPageAndFilter(Predicate predicate,
                                                 Pageable pageable) {

        return ownerRepository.findAll(predicate, pageable)
                .stream()
                .map(ownerMapper::toDto)
                .toList();
    }

    public List<OwnerDto> findByName(String ownerName) {
        return ownerRepository.findByName(ownerName).stream().map(ownerMapper::toDto).toList();
    }

    public OwnerDto findOne(Long id) {
        return ownerMapper.toDto(ownerRepository.findById(id).orElseThrow(OwnerNotFoundException::new));
    }

    public OwnerDto create(OwnerDto ownerDto) {
        return ownerMapper.toDto(ownerRepository.save(ownerMapper.toEntity(ownerDto)));
    }

    public void delete(Long id) {
        ownerRepository.findById(id).orElseThrow(OwnerNotFoundException::new);
        ownerRepository.deleteById(id);
    }

    public OwnerDto updateOwner(OwnerDto ownerDto, Long id) {
        if (!Objects.equals(ownerDto.id(), id)) {
            throw new OwnerIdMismatchException();
        }

        ownerRepository.findById(id).orElseThrow(OwnerNotFoundException::new);

        return ownerMapper.toDto(ownerRepository.save(ownerMapper.toEntity(ownerDto)));
    }
}
